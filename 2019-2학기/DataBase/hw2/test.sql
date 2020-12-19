select distinct course_id
from section
where semester = 'Fall' and year = 2009 and course_id not in
(select course_id
from section
where semester = 'Spring' and year = 2010);

select not FALSE;

select avg(salary)
from instructor
where instructor.dept_name = 'Comp. Sci.';

select count(distinct ID)
from teaches
where semester = 'Spring' and year = 2010;

select dept_name, avg(salary) as avg_sal
from instructor
group by dept_name;

select dept_name, ID, avg(salary)
from instructor
group by dept_name;

select dept_name, avg(salary)
from instructor
group by dept_name
having avg(salary) != 42000;

select name
from instructor
where salary > all(select salary from instructor where dept_name != 'Biology');

select s.course_id
from section as s
where s.semester = 'Fall' and year = 2009 and exists(select * from section as t where t.semester = 'Spring' and year = 2010 and t.course_id = s.course_id);

select distinct course_id
from section
where semester = 'Fall' and year = 2009 and course_id in
(select course_id
from section
where semester = 'Spring' and year = 2010);


select s.name
from student as s 
where not exists  
(select cs.course_id
from course as cs
where cs.dept_name = 'Elec. Eng.' and cs.course_id not in
(select t.course_id
from takes as t, course as c
where s.id = t.id and t.course_id = c.course_id and c.dept_name = 'Elec. Eng.'));
## 증요

select cs.course_id
from course as cs
where cs.dept_name = 'Biology';

(select *
from student as s , takes as t, course as c
where s.id = t.id and t.course_id = c.course_id and c.dept_name = 'Comp. Sci.');

select distinct S.ID, S.name
from student as S
where not exists ((select C1.course_id from course as C1 where C1.dept_name = 'Biology' and C1.course_id not in 
(select t.course_id from takes as t where S.ID = t.ID)));

select distinct s.course_id
from section as s
where 1 >= (select count(t.course_id)
			from section as t
			where t.course_id = s.course_id and t.year = 2009); 
## 중요


select dept_name, avg_salary
from (select dept_name, avg(salary) as avg_salary from instructor group by dept_name)
where avg_salary > 42000;

select avg(salary)
from instructor
where dept_name = 'Comp. Sci.';


select distinct dept_name
from instructor join department using (dept_name)
group by dept_name
having sum(salary) > 
(select avg(summation)
from (
select sum(salary) as summation
from instructor join department using (dept_name)
group by dept_name) as t);

update instructor
set salary = case when salary <= 100000 then salary * 1.03 else salary * 1.05 end;


select case when 'a' = 'a' then 1
			when 'a' = 'b' then 2
            when 'a' = 'c' then 3
            else 4 end as caser;

create table dummy
select * from student;

update dummy as S
set tot_cred = (select coalesce(sum(credits), 0)
				from takes natural join course
				where S.ID = takes.ID and takes.grade <> 'F' and takes.grade is not null);
                
select * from student join takes on student.ID = takes.ID;

select * from course natural right outer join prereq;

select * from course join prereq on course.course_id = prereq.course_id;

create view dummy as
select ID, name, dept_name from instructor;

select * from dummy;

create view dep_tot_sal as
select dept_name, sum(salary)
from (department join instructor using (dept_name));


create view faculty as
select ID, name, dept_name
from instructor;

insert into faculty values('30765','Green','Music');

create view instructor_info as
select ID, name, building
from instructor, department
where instructor.dept_name = department.dept_name;

insert into instructor_info values('69987', 'White', 'Taylor');

create view history_instructors as
select *
from instructor
where dept_name = 'History';

insert into history_instructors values('25556', 'Brown', 'Biology', 100000);

create table dummy2
(course_id varchar(8), sec_id varchar(8), semester varchar(6),
primary key (course_id, sec_id),
constraint checking check(semester in ('Fall', 'Winter', 'Summer', 'Spring')));

create table dummy3
(course_id varchar(8), sec_id varchar(8), age real, height double,
 primary key (age),
 foreign key (course_id) references dummy2(course_id));
 
create table dummy4
(course_id varchar(8), sec_id varchar(8), age real, height double, name varchar(20),
 primary key (age),
 foreign key (course_id) references dummy2(course_id),
 unique (name));

create table dummy5
(name varchar(20), ID integer,
 primary key (ID),
 foreign key (name) references dummy4(name));
 
 create table dummy6
(name varchar(20), ID integer, height double,
 primary key (ID),
 foreign key (name) references dummy5(name));

 create table dummy7
(name varchar(20), ID integer not null, height double default 7,
 primary key (ID),
 foreign key (name) references dummy5(name));
 
 
create table person(
ID char(10),
name char(40),
mother char(40),
father char(40),
primary key (ID),
unique (name),
foreign key (father) references person (name),
foreign key (mother) references person (name));


create index studentID_index on student(ID);

grant all privileges on student to public;

grant references(dept_name) on department to Mariano; 


DELIMITER $$
create function dept_countf (idept_name varchar(20))
returns integer
begin
	declare num integer default 0;
    select count(distinct id) into num
    from instructor
    where dept_name = idept_name;
    return num;
end $$
DELIMITER ;

DELIMITER $$
create procedure dept_countp (in idept_name varchar(20), out dnum integer)
begin
	set dnum = 0;
    select count(distinct id) into dnum
    from instructor
    where dept_name = idept_name;
end $$
DELIMITER ;

select A.ID, A.salary, (1 + (select count(*) from instructor B where coalesce(B.salary,0) > coalesce(A.salary,0))) as s_rank
from instructor A
order by s_rank;

select distinct ID, name
from instructor;

create table dummy8(
	avalue integer,
    bvalue integer);
    
insert into dummy8 values(1,2);
insert into dummy8 values(1,3);
insert into dummy8 values(2,3);
select * from dummy8;