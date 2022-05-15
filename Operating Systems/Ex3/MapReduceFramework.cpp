
#include <atomic>
#include <mutex>
#include <iostream>
#include "Barrier.h"
#include "MapReduceFramework.h"

#define SYSTEM_ERR "system error: "

#define THREAD_ERR "Thread creation error exiting from program"

#define ALLOC_ERR "Bad Allocation Exiting from program"

#define SHUFFLE_BASE_STATUS 0b1000000000000000000000000000000000000000000000000000000000000000ull

#define SIZE_SHIFT 31ull

#define STAGE_SHIFT 62ull

#define REDUCE_BASE_STATUES 13835058055282163712ull

#define MUTEX_ERR "Mutex Error exiting program"

struct JobContext;


/** A typedef for the vector of <K2,V2> pairs. */
typedef std::vector<IntermediatePair> IntermediateVec;

/**
 * The struct/class for the thread context object.
 */
typedef struct ThreadContext
{
    /**
     * The c'tor for the threadcontext object.
     * @param threadNum The number of threads.
     * @param inputVec The given input vector.
     * @param outVec The vector to output to.
     * @param client A client object to run the job.
     * @param counter An atomic counter.
     * @param jContext A jobcontext object.
     * @param barrier A barrier for the threads.
     */
	ThreadContext(int threadNum, const InputVec &inputVec, OutputVec *outVec, const MapReduceClient &client,
	        std::atomic<int> *counter, JobContext *jContext, Barrier *barrier)
			: threadNum(threadNum), input_vec(inputVec), out_vec(outVec), client(client), jContext(jContext),
			counter(counter), barrier(barrier)
	{
		try
		{
			inter_vec = new IntermediateVec(0);
		} catch (std::bad_alloc &e)
		{
			std::cerr << SYSTEM_ERR << ALLOC_ERR << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	int threadNum;
	const InputVec &input_vec;
	std::mutex mutex_inter_vec;
	OutputVec *out_vec;
	const MapReduceClient &client;
	IntermediateVec *inter_vec;
	JobContext *jContext;
	std::atomic<int> *counter;
	Barrier *barrier;


} ThreadContext;

/**
 * The struct/class which will act as the job handler.
 */
typedef struct JobContext
{
    /**
     * The C'tor to create a jobcontext object.
     * @param threads An array of threads.
     * @param tContexts An array of thread contexts.
     * @param inter_map An array of keys and values.
     */
    JobContext(pthread_t *threads, std::vector<ThreadContext *> *tContexts, IntermediateMap *inter_map)
            : threads(threads), tContexts(tContexts), status(0), inter_map(inter_map),
            k2_vec(std::vector<K2 *>(0)) {}

    /**
     * A D'tor for the jobcontext object.
     */
    ~JobContext()
    {
        delete (*tContexts)[0]->counter;
        delete (*tContexts)[0]->barrier;
        delete inter_map;
        for (auto &t:*tContexts)
        {
            delete t->inter_vec;
            delete t;
        }
        delete tContexts;
        free(threads);
    }

    pthread_t *threads;
    std::vector<ThreadContext *> *tContexts;
    std::atomic<u_int64_t> status;
    IntermediateMap *inter_map;
    std::vector<K2 *> k2_vec;
} JobContext;

/**
 * A function which gets the left bits in the number that represent the current stage.
 * @param a The number that we wish to extract the stage from.
 * @return The number of the current stage.
 */
u_int64_t get_left_bits(u_int64_t a)
{
	return (unsigned int) (a >> STAGE_SHIFT);
}

/**
 * A function which gets the 31 middle bits in the number that represent the total values.
 * @param a The number that we wish to extract the total values from.
 * @return The number of total values.
 */
u_int64_t get_31_mid_bits(u_int64_t a)
{
	return (unsigned int) ((a << 2ull) >> 33ull);
}

/**
 * A function which gets the 31 right bits in the number that represent the current values.
 * @param a The number that we wish to extract the current values from.
 * @return The number of current values.
 */
u_int64_t get_31_right_bits(u_int64_t a)
{
	return (unsigned int) ((a << 33ull) >> 33ull);
}

/**
 * A function which returns a K2 and V2 pair.
 * @param key A key.
 * @param value A value.
 * @param context An optional context.
 */
void emit2(K2 *key, V2 *value, void *context)
{
	auto vec = (IntermediateVec *) context;
	vec->push_back(IntermediatePair(key, value));
}

/**
 * A function which returns a K3 and V3 pair.
 * @param key A key.
 * @param value A value.
 * @param context An optional context.
 */
void emit3(K3 *key, V3 *value, void *context)
{
	auto vec = (OutputVec *) context;
	vec->push_back(OutputPair(key, value));
}

/**
 * A function that the threads of map will use.
 * @param context The context we will be working on.
 */
void map(ThreadContext *context)
{
	int input_size = context->input_vec.size();
	u_int64_t tmp = 1, tmp2 = input_size;
	if (*(context->counter) == 0)
	{
		(context->jContext->status) |= ((tmp << STAGE_SHIFT) + (tmp2 << SIZE_SHIFT));
	}
	while ((int) context->counter->load() < input_size)
	{
		int old_value = (*(context->counter))++;
		if (old_value >= input_size)
		{
			break;
		}
		auto p = context->input_vec[old_value];
		try
		{
			context->mutex_inter_vec.lock();
		} catch (std::system_error &e)
		{
			std::cerr << SYSTEM_ERR << MUTEX_ERR << std::endl;
			exit(EXIT_FAILURE);
		}
		context->client.map(p.first, p.second, context->inter_vec);
		context->mutex_inter_vec.unlock();
		context->jContext->status++;
	}
	context->barrier->barrier();
}

/**
 * A function that the single shuffle thread will run.
 * @param context The context we will be working on.
 */
void shuffle(ThreadContext *context)
{
	u_int64_t total_values = 0, values_done = 0;
	IntermediateMap *inter_map = context->jContext->inter_map;
	auto threadcontxt = context->jContext->tContexts;
	while ((float(get_31_right_bits(context->jContext->status)) /
	(float) get_31_mid_bits(context->jContext->status)) !=1)
	{
		for (int i = 0; i < (int) threadcontxt->size() - 1; ++i)
		{
			IntermediateVec *t_vec = threadcontxt->at(i)->inter_vec;
			try
			{
                		threadcontxt->at(i)->mutex_inter_vec.lock();
			} catch (std::system_error &e)
			{
				std::cerr << SYSTEM_ERR << MUTEX_ERR << std::endl;
				exit(EXIT_FAILURE);
			}
			if (!t_vec->empty())
			{
				for (IntermediatePair p:*t_vec)
				{
					if (inter_map->find(p.first) == inter_map->end())
					{
						inter_map->insert({p.first, std::vector<V2 *>()});
					}
					inter_map->at(p.first).push_back(p.second);
					values_done++;
					total_values++;
				}
				t_vec->clear();
			}
                threadcontxt->at(i)->mutex_inter_vec.unlock();
		}
	}
	context->barrier->barrier();
	context->jContext->status = SHUFFLE_BASE_STATUS;
	for (auto t: *context->jContext->tContexts)
	{
		total_values += t->inter_vec->size();
	}
	context->jContext->status |= ((values_done) | (total_values << SIZE_SHIFT));
	for (int i = 0; i < (int) threadcontxt->size() - 1; ++i)
	{
		IntermediateVec *t_vec = threadcontxt->at(i)->inter_vec;
		for (IntermediatePair p:*t_vec)
		{
			if (inter_map->find(p.first) == inter_map->end())
			{
				inter_map->insert({p.first, std::vector<V2 *>()});
			}
			inter_map->at(p.first).push_back(p.second);
			context->jContext->status++;
		}
	}
	for (auto &p:*inter_map)
	{
		context->jContext->k2_vec.push_back(p.first);
	}
	(*context->counter) = 0;
	auto tmp = (u_int64_t) context->jContext->k2_vec.size();
	context->jContext->status = REDUCE_BASE_STATUES;
	context->jContext->status += (tmp << SIZE_SHIFT);
}

/**
 * A function that the threads of reduce will use.
 * @param context The context we will be working on.
 */
void reduce(ThreadContext *context)
{
	while (get_31_right_bits(context->jContext->status) < context->jContext->k2_vec.size())
	{
		int old_value = (*(context->counter))++;
		if (old_value >= (int) context->jContext->inter_map->size())
		{
			break;
		}
		auto key = context->jContext->k2_vec[old_value];
		try
		{
			context->jContext->tContexts->at(0)->mutex_inter_vec.lock(); //Reuse of mutex of thread 0
		} catch (std::system_error &e)
		{
			std::cerr << SYSTEM_ERR << MUTEX_ERR << std::endl;
			exit(EXIT_FAILURE);
		}
		auto vec = context->jContext->inter_map->at(key);
		context->client.reduce(key, vec, (void *) context->out_vec);
		context->jContext->tContexts->at(0)->mutex_inter_vec.unlock();
		context->jContext->status++;
	}

}

/**
 * A function that gives the jobs to the threads.
 * @param t_context The context we are working on.
 * @return nullptr.
 */
void *ThreadRunningFunc(void *t_context)
{
	auto context = (ThreadContext *) t_context;
	if (context->threadNum == (int) context->jContext->tContexts->size() - 1)
	{
		shuffle(context);
	}
	else
	{
		map(context);
	}
	context->barrier->barrier();
	reduce(context);
	return nullptr;
}

/**
 * A function that start the process.
 * @param client The given client object to run.
 * @param inputVec The given input vector.
 * @param outVec The vector to output to.
 * @param multiThreadLevel The amount of threads to run.
 * @return The jobhandler.
 */
JobHandle startMapReduceJob(const MapReduceClient &client, const InputVec &inputVec,
        OutputVec &outputVec, int multiThreadLevel)
{
	try
	{
		auto threads = (pthread_t *) malloc(sizeof(pthread_t) * multiThreadLevel);
		if (threads == nullptr)
		{
			std::cerr << SYSTEM_ERR << ALLOC_ERR << std::endl;
			exit(EXIT_FAILURE);
		}
		auto counter = new std::atomic<int>(0);
		auto barrier = new Barrier(multiThreadLevel);
		auto tContexts = new std::vector<ThreadContext *>(multiThreadLevel);
		auto inter_map = new IntermediateMap();
		auto jContext = new JobContext(threads, tContexts, inter_map);

		for (int i = 0; i < multiThreadLevel - 1; ++i)
		{
			(*tContexts)[i] = new ThreadContext(i, inputVec, &outputVec, client, counter, jContext, barrier);
			if (pthread_create(threads + i, nullptr, ThreadRunningFunc, (void *) ((*tContexts)[i])) != 0)
			{
				std::cerr << SYSTEM_ERR << THREAD_ERR << std::endl;
				exit(EXIT_FAILURE);
			};

		}
		(*tContexts)[multiThreadLevel - 1] = (new ThreadContext(multiThreadLevel - 1, inputVec, &outputVec, client,
				counter,
				jContext,
				barrier)); // shuffle Thread
		if (pthread_create(threads + multiThreadLevel - 1, nullptr, ThreadRunningFunc,
				(void *) ((*tContexts)[multiThreadLevel - 1])) != 0)
		{
			std::cerr << SYSTEM_ERR << THREAD_ERR << std::endl;
			exit(EXIT_FAILURE);
		};

		return (JobHandle) jContext;
	}
	catch (std::bad_alloc &e)
	{
		std::cerr << SYSTEM_ERR << ALLOC_ERR << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * A function that gets the job handle returned by startMapReduceFramework and,
 * waits until it is finished.
 * @param job The given job.
 */
void waitForJob(JobHandle job)
{
	auto j = (JobContext *) job;
	JobState state;
	getJobState(job, &state);
	for (int i = 0; i < (int) j->tContexts->size(); ++i)
	{
		pthread_join((j->threads)[i], nullptr);
	}
}

/**
 * A function that gets a job handle and updates the state of the job into the given JobState struct.
 * @param job The given job.
 * @param state The state of the job.
 */
void getJobState(JobHandle job, JobState *state)
{
	auto j = (JobContext *) job;
	u_int64_t status = j->status;
	auto c = (float) get_31_mid_bits(status);
	float numerator = c > 0 ? c : 1;
	state->percentage = (float(get_31_right_bits(status)) / numerator) * 100;
	state->stage = stage_t(get_left_bits(status));
}

/**
 * A function that releases all resources of a job.
 * @param job The current job.
 */
void closeJobHandle(JobHandle job)
{
	JobState state;
	waitForJob(job);
	getJobState(job, &state);
	if (state.stage == REDUCE_STAGE && state.percentage == 100)
	{

		delete (JobContext *) job;
	}
}

