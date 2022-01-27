import json
import re
import datetime
from collections import Counter
from pyspark import SparkContext

# A hack to avoid having to pass 'sc' around
dummyrdd = None
def setDefaultAnswer(rdd): 
	global dummyrdd
	dummyrdd = rdd


def task1(amazonInputRDD):
        t_array = amazonInputRDD.map(lambda line: tuple(re.sub('[a-z]+', "", line).split(" ")))
        return t_array


def task2(amazonInputRDD):
        t_array = amazonInputRDD.\
                map(lambda line: tuple(re.sub(' \w+ ', " ", line).split(" "))).\
                map(lambda tup: (tup[0], (float(tup[1]), 1))).\
                reduceByKey(lambda a, b: (a[0]+b[0], a[1]+b[1])).\
                map(lambda t_up: (t_up[0], t_up[1][0]/t_up[1][1]))
        return t_array


def task3(amazonInputRDD):
        t_array = amazonInputRDD. \
                map(lambda line: tuple(re.sub('\w+ ', "", line, 1).split(" "))).\
                map(lambda t_up: (t_up[0], float(t_up[1]))).\
                groupByKey().mapValues(list). \
                map(lambda t_up: (t_up[0], Counter(t_up[1]))).\
                map(lambda t_up: (t_up[0], max([key for key in t_up[1].keys() if t_up[1][key] == max(t_up[1].values())])))
        return t_array


def task4(logsRDD):
        t_array = logsRDD. \
                map(lambda line: (int(re.search('[0-9][0-9][0-9][0-9]', tuple(line.split("- - ["))[1]).group(0)), 1)). \
                reduceByKey(lambda a, b: a + b)
        return t_array


def task5_flatmap(x):
        word_arr = x.split(" ")
        for i in range(len(word_arr)):
                word_arr[i] = re.sub('/|\W', "", word_arr[i])
        return word_arr


def task6(playRDD):
        t_array = playRDD. \
                map(lambda line: (line.split(" ")[0], (line, len(line.rstrip(" ").split(" "))))).\
                filter(lambda t_up: t_up[1][1] > 10)
        return t_array


def task7_flatmap(x):
        word_array = []
        for i in range(len(x["laureates"])):
                word_array.append(x["laureates"][i]["surname"])
        return word_array


def task8(nobelRDD):
        t_array = nobelRDD.map(json.loads). \
                map(lambda x: (x.get("category"), [x.get("laureates")[i]["surname"] for i in range(len(x["laureates"]))])).\
                reduceByKey(lambda a, b: a + b)
        return t_array

def task9(logsRDD, l):
        t_array = logsRDD. \
                map(lambda line: func2(line, l)) \
                .reduceByKey(lambda a, b: a+b)\
                .filter(lambda tup: func3(tup, l)).\
                map(lambda tup: tup[0])
        return t_array

def task10(bipartiteGraphRDD):
        t_array = bipartiteGraphRDD. \
                map(lambda tup: (tup[0], [tup[1]])). \
                reduceByKey(lambda a, b: a + b). \
                map(lambda tup: (len(tup[1]), 1)).\
                reduceByKey(lambda a, b: a + b)
        return t_array


def func2(line, l):
        spl_line = line.split(" ")
        host = spl_line[0]
        long_date = spl_line[3]
        date = long_date[1:12]
        if date in l:
                return (host, [date])
        return (host, [])


def func3(tup, l):
        flag = True
        for d in l:
                if d not in tup[1]:
                        flag = False
                        break
        return flag
