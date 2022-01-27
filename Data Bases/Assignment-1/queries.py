queries = ["" for i in range(0, 25)]

### 0. List all the users who were born in 1998.
### Output column order: userid, name, birthdate, joined
### Order by birthdate ascending
queries[0] = """
select userid, name, birthdate, joined
from users
where extract(year from birthdate) = 1998 
order by birthdate asc;
"""

### 1. Report the user information (userid, name, birthdate, joined) for all users with first name 'Carol'. (Hint: use ``like'').
### Output column order: userid, name, birthdate, joined
### Order by birthdate ascending
queries[1] = """
select userid, name, birthdate, joined
from users
where name like 'Carol%'
order by birthdate asc;
"""


### 2. Write a query to output the age (in years) for every user when they joined the social network.
### Use 'age' function that operates on dates (https://www.postgresql.org/docs/12/functions-datetime.html)
### Output columns: name, age
### Order output by age increasing
### The first row in the output should be: (Jason Phillips, 6)
queries[2] = """
select name, date_part('year', age(joined, birthdate)) as age
from users
order by age asc;
"""

### 3. Select all the "distinct" years that users with names starting with 'M' are born in.
### Output column: year
### Order output by year ascending
queries[3] = """
select DISTINCT date_part('year', birthdate) as year
from users
where name like 'M%'
order by year asc;
"""

### 4. Write a query to find users who have been on the social network for more than half their life as of 'August 31, 2021'.
### Use 'age' function as above.
### Output columns: name
### Order by name ascending
queries[4] = """
select name
from users
where age('2021-08-31', joined) >= 0.5*age('2021-08-31', birthdate)
order by name asc;
"""

### 5. Write a single query to report all status updates for  the user 'Kevin Allen' by joining status and user.
### Output Column: status_time, text
### Order by: status_time increasing
queries[5] = """
select status_time, text
from users natural join status
where users.name = 'Kevin Allen'
order by status_time asc;
"""

### 6. Write a query to find all users who satisfy one of the following conditions:
###        - the user's name starts with a 'J' and they were born before and including 1980
###        - the user's name starts with an 'M' and they were born after 1980 (excluding 1980)
### Output columns: name, birthdate
### Order by: name ascending
queries[6] = """
select name , birthdate
from users
where (name like 'J%' and date_part('year', birthdate) <= 1980) or (name like 'M%' and date_part('year', birthdate) > 1980)
order by name asc;
"""


### 7. Count the number of the friends of the user 'Kevin Allen'.
### Output columns: num_friends
queries[7] = """
select count(*) as num_friends
from friends
where userid1 = 'user90';
"""

### 8. Count the total number of users whose names start with a vowel ('A', 'E', 'I', 'O', 'U'). (Hint: Use "in" and "substr").
### Output columns: numusers
queries[8] = """
select count(userid) as numusers
from users
where substring(users.name, 1, 1) in ('A', 'E', 'I', 'O', 'U');
"""


### 9. SQL "with" clause can be used to simplify queries. It essentially allows
### specifying temporary tables to be used during the rest of the query. See Section
### 3.8.6 (6th Edition) for some examples.
###
### Write a query to find the name(s) of the user(s) with the largest number of friends. We have provided
### a part of the query to build a temporary table.
###
### Output columns: name, num_friends 
### Order by name ascending (there may be more than one answer)
queries[9] = """
with temp as (
        select name, count(*) as num_friends 
        from users, friends 
        where users.userid = friends.userid1 
        group by name)
select *
from temp
where num_friends = (select max(num_friends) from temp)
order by name asc;
"""



### 10. List the names of the users who posted no status updates. Hint: Use "not in".
### Output Columns: name
### Order by name ascending
queries[10] = """
select name
from users
where users.userid not in (select userid from status)
order by name asc;
"""


### 11. Write a query to output a list of users and their friends, such that the friend has an
### upcoming birthday within next 15 days.  Assume today is Sept 10, 2021
### (so look for birthdays between Sept 11 and Sept 25, both inclusive). You can hardcode
### that if you'd like.
### Output: username, friendname, friendbirthdate
### Order by: username, friendname ascending
queries[11] = """
with temp as (
select userid, name, birthdate
from users)
select users.name, temp.name, temp.birthdate
from users join friends on (userid = userid1) join temp on (userid2 = temp.userid)
where friends.userid2 = temp.userid and date_part('month', temp.birthdate) = 9 and date_part('day', temp.birthdate) > 10 and date_part('day', temp.birthdate) < 26
order by users.name, temp.name asc;
"""


### 12. For each user who has posted at least two status updates, count the
### average amount of time between his or her status updates in seconds.
### Order the results in the increasing order by the userid.
###
### Hint: Date substraction returns the amount in (fractional) number of days. 
### Hint 2: The number of seconds in a day is 86400.
### Hint 3: Use "having" to find users who have at least 2 status updates.
###
### Output columns: userid, gapseconds
### Order by: userid
queries[12] = """
with temp as (
select userid, count(statusid) as num_status, max(status_time) as max_date, min(status_time) as min_date
from status
group by userid 
having count(statusid) >= 2)
select distinct userid, (86400*(max_date - min_date))/(num_status-1) as diff
from status natural join temp
where status.userid = temp.userid
order by userid;
"""


### 13. Generate a list - (birthyear, num-users) - containing the years
### in which the users in the database were born, and the number of users
### born in each year.
###
### You don't need to worry about years where no users were born, i.e., there should
### not be any entries for years where no one in the database was born.
###
### Output columns: birthyear, num_users
### Order by birthyear
queries[13] = """
select date_part('year', birthdate) as birthyear, count(userid)
from users
group by birthyear
order by birthyear;
"""

### 14. Generate a list - (birthyear, num-users) - containing the years
### in which the users in the database were born, and the number of users
### born in each year.
###
### However, your output here should have all years between 1940 and 2000 both inclusive, 
### with 0 counts if no users were born that year
###
### HINT: Use "generate_series()" to create an inline table -- try 
### "select * from generate_series(1, 10) as g(n);" to see how it works.
### This is what's called a "set returning function", and the result can be used as a relation.
### See: https://www.postgresql.org/docs/12/functions-srf.html
###
### Output columns: birthyear, num_users
### Order by birthyear
queries[14] = """
with temp as (select * 
from generate_series(1940, 2000, 1) as g(n))
select temp.n, count(date_part('year', users.birthdate)) as num
from users full outer join temp on (date_part('year', users.birthdate) = temp.n)
group by temp.n
order by temp.n;
"""

### 15. Find the name of the group with the maximum number of members. There may be more than one answer.
###
### Output: name, num_members
### Order by name ascending
queries[15] = """
with temp as (select name, count(userid) as n_members
from groups join members on (groups.groupid=members.groupid)
group by groups.name
order by name asc)
select name, n_members
from temp
where n_members = (select max(n_members) from temp)
order by name asc;
"""

### 16. Write a query to find the names of all users that "Michael Smith" is friends with or follows.
### The output should be a list of names, with a second column that takes values: "friends", or "follows"
### Some names might appear twice
###
### HINT: Use "union"
### HINT 2: Note that constants can be hardcoded in the select cause (e.g., try "select userid, 'hi' from users;")
###
### Output columns: name, type -- type takes values "friends" or "follows"
queries[16] = """
with temp as (select friends.userid2 as userid, 'friends' as type
from users join friends on (users.userid = friends.userid1)
where users.name like 'Michael Smith%'
union
select follows.userid2 as userid, 'follows' as type
from users join follows on (users.userid = follows.userid1)
where users.name like 'Michael Smith%')
select name, type
from users join temp on (users.userid = temp.userid)
order by users.name;
"""


### 17. Write a query to count for each user, the number of other users that they are both friends with and follow.
###
### HINT: Use "intersection" on "friends" and "follows" as a starting point.
###
### Output columns: userid, name, num_common
### Order by name ascending
queries[17] = """

with temp as (select userid1, userid2
from friends
intersect 
select userid1, userid2
from follows)
select users.userid, users.name, count(temp.userid2)
from users join temp on (users.userid = temp.userid1)
group by users.userid, users.name
order by users.name; 
"""

### 18. Find the pairs of users that were born closest to each other (there are no two users with the
### same birthdate). There may be multiple answers.
###
### For any pair, the output should only contain one row such that username1 < username2
###
### Output columns: username1, username2 
### Order by: username1
queries[18] = """
with temp as (
select name as username1, lag(name) over (order by birthdate) as username2, birthdate as b1 
from users
),
temp2 as (select username1, username2, min(temp.b1 - users.birthdate) as diff
from temp join users on (temp.username2 = users.name)
group by username1, username2)
select username1, username2
from temp2
where diff = (select min(diff) from temp2)
order by username1;
"""

### 19. For each user, calculate the number of friends and the number of followers.
### Note that a user with no friends will not appear in the friends table, and same
### goes for follows table. But 0 counts should still be appropriately recorded.
###
### Output columns: userid, name, num_friends, num_followers
### Order by: userid
queries[19] = """
with temp as (
select users.userid, count(follows.userid2) as num_follows
from users left join follows on (users.userid = follows.userid2)
group by users.userid
order by users.userid),
temp2 as (
select users.userid, count(friends.userid1) as num_friends
from users left join friends on (users.userid = friends.userid1)
group by users.userid
order by users.userid)
select users.userid, users.name, temp2.num_friends, temp.num_follows
from users join temp on (users.userid = temp.userid) join temp2 on (users.userid = temp2.userid)
order by users.userid;
"""


### 20. Find all users who are not followed by anyone who is a member of the group: 
### 'University of Maryland, College Park USA'
### You can hard code the group id: 'group36'
###
### HINT: Use Set Operation "Not Exists"
###
### Output column order: name
### Order by name ascending
queries[20] = """
with temp as (
select users.name, count(follows.userid2) as num_total
from users left join follows on (users.userid = follows.userid2) left join members on (follows.userid1 = members.userid)
group by users.name
order by users.name),
temp2 as (
select userid, groupid
from members
where members.groupid not like 'group36%'
order by userid),
temp3 as (
select users.name, count(follows.userid2) as num_without
from users left join follows on (users.userid = follows.userid2) left join temp2 on (follows.userid1 = temp2.userid)
group by users.name
order by users.name)
select users.name
from users left join temp on (users.name=temp.name) left join temp3 on (users.name=temp3.name)
where num_total = num_without
order by users.name asc;
"""


### 21. Create a copy of the "users" table using the following command:
### select * into userscopy from users, OR
### create table userscopy as (select * from users)
###
### For the next few questions, we will use this duplicated table
###
### Write a single query/statement to add two new columns to the "users" table -- age (integer), and usage (varchar(10)).
queries[21] = """
alter table userscopy
add column age integer,
add column usage varchar(10);
"""

### 22. Write a single query/statement to set the values of both of the new columns. Use "age()" function to find the 
### age in years (as we did above) as of "August 31, 2021". 
###
### The "usage" column takes three values: "heavy", "medium", "light"
### The usage is "heavy" if the total number of status updates > 10, "medium" if 5 <= # status updates <= 10, and "light" if < 5.
###
### HINT: Use a "with" clause to create a temporary table with the counts, and use CASE to simplify the update.
###
### NOTE: You need to account for users with no status updates in 'status' table
queries[22] = """
with temp as (
select users.userid, count(status.statusid) as num_status
from  users left join status on (users.userid = status.userid)
group by users.userid
order by users.userid)
update userscopy
set age = date_part('year', age('2021-08-31',birthdate)),
usage = case
when temp.num_status between 5 and 10 then 'medium'
when temp.num_status > 10 then 'heavy'
else 'light'
end
from temp
where userscopy.userid = temp.userid;
"""


### 23. Write a query "delete" all users from userscopy who were born in May.
queries[23] = """
delete from userscopy
where date_part('month', birthdate) = 5;
"""


### 24. Use "generate_series" as above to write a single statement to insert 10 new tuples
### to 'userscopy' of the form:
### ('newuser11', 'New User 11', '1990-01-11', '2015-01-11', 0, 'light')
### ('newuser12', 'New User 12', '1990-01-12', '2015-01-12', 0, 'light')
### ...
### ('newuser20', 'New User 20', '1990-01-20', '2015-01-20', 0, 'light')
###
### HINT: Use concatenation operator: 'newuser' || 0, and addition on dates to simplify.
queries[24] = """
insert into userscopy(userid, name, birthdate, joined, age, usage)
select 'newuser' || i::text, 'New User ' || i::text, i + '1989-12-31'::date, i + '2014-12-31'::date, '0', 'light'
from generate_series(11, 20) as t(i);
"""
