create table department(
	dept_name varchar(20),
    building varchar(30),
    budget numeric(15,0),
    primary key (dept_name));

create table instructor(
	ID		char(5),
    name	varchar(20) not null,
    dept_name varchar(20),
    salary	numeric(8,2),
    primary key (ID),
	foreign key (dept_name) references department(dept_name)
      on delete set null);

create table student(
	ID		varchar(5),
    name 	varchar(20) not null,
    dept_name varchar(20),
    tot_cred numeric(3,0),
    primary key (ID),
    foreign key (dept_name) references department(dept_name)
      on delete set null);


create table section(
	course_id varchar(8),
    sec_id varchar(8),
    semester varchar(6),
    year numeric(4,0),
    building varchar(30),
    room_no int,
    time_slot_id varchar(10),
    primary key(course_id, sec_id, semester, year, building));

create table takes(
	ID		varchar(5),
    course_id	varchar(8),
    sec_id		varchar(8),
    semester	varchar(6),
    year		numeric(4,0),
    grade		varchar(2),
    primary key (ID, course_id, sec_id, semester, year),
    foreign key (ID) references student(ID),
    foreign key (course_id, sec_id, semester, year) 
		references section(course_id, sec_id, semester, year));
        
create table course(
	course_id	varchar(8),
    title		varchar(50),
    dept_name	varchar(20),
    credits		numeric(2,0),
    primary key (course_id),
    foreign key (dept_name) references department(dept_name));


insert into instructor values('10211', 'Smith', 'Biology', 66000);

delete from student where dept_name = 'Biology';

select name
from instructor;


    
select distinct dept_name
from instructor;



select all dept_name
from instructor;


select * from instructor;



select ID, name, salary/12
from instructor;


select name
from instructor
where dept_name = 'Comp. Sci.' and salary > 80000;

select *
from instructor, teaches;

select name, course_id
from instructor, teaches
where instructor.ID = teaches.ID;

select name, course_id
from instructor natural join teaches;


select section.course_id, semester, year, title
from section, course
where section.course_id = course.course_id and
	  dept_name = 'Comp. Sci.';
      
select * 
from instructor natural join teaches;


select name, course_id
from instructor, teaches
where instructor.ID = teaches.ID;

select name, course_id
from instructor natural join teaches;

select ID, name, salary/12 as monthly_salary
from instructor;


select distinct T.name
from instructor as T, instructor as S
where T.salary > S.salary and S.dept_name = 'Comp. Sci.';

select name
from instructor
where name like '%dar%';

select distinct name
from instructor
order by dept_name, name;


select name
from instructor
where salary between 90000 and 100000;

select name, course_id
from instructor, teaches
where (instructor.ID, dept_name) = (teaches.ID, 'Biology');

(select course_id from section where semester = 'Fall' and year = 2009)
union
(select course_id from section where semester = 'Spring' and year = 2010);

select course_id from section 
where (semester = 'Fall' and year = 2009) 
and course_id in
(select course_id from section
where (semester = 'Spring' and year = 2010));

select course_id from section 
where (semester = 'Fall' and year = 2009)
and course_id not in 
(select course_id from section
where (semester = 'Spring' and year = 2010));

select name
from instructor
where salary is null;

select avg(salary)
from instructor
where dept_name= 'Comp. Sci.';

select count(distinct ID)
from teaches
where semester = 'Spring' and year = 2010;

select count(*)
from course;


select dept_name, avg (salary) as avg_salary
from instructor
group by dept_name;

select dept_name, ID, avg(salary)
from instructor
group by dept_name;

select dept_name, avg(salary)
from instructor
group by dept_name
having avg(salary) > 42000;

select sum(salary)
from instructor;

select distinct course_id
from section
where (semester = 'Fall' and year = 2009) and
	course_id in (select course_id
					from section
					where semester = 'Spring' and year = 2010);

select distinct course_id
from section
where (semester = 'Fall' and year = 2009) and
	course_id not in (select course_id
					from section
					where semester = 'Spring' and year = 2010);
                    
select count(distinct ID)
from takes
where (course_id, sec_id, semester, year) in
	(select course_id, sec_id, semester, year
	 from teaches
	where teaches.ID = 10101);
    
select distinct T.name
from instructor as T, instructor as S
where T.salary > S.salary and S.dept_name = 'Biology';

select name
from instructor
where salary > some(select salary
					from instructor
					where dept_name = 'Biology');

select name
from instructor
where salary > all(select salary
					from instructor
					where dept_name = 'Biology');
                    
select course_id
from section as S
where semester = 'Fall' and year = 2009 and
	exists(select *
		from section as T
		where semester = 'Spring' and year = '2010' and
		S.course_id = T.course_id);
        
select distinct S.ID, S.name
from student as S
where not exists (select course_id
					from course
					where dept_name = 'Biology'
                    and course_id not in
                    (select T.course_id
                    from takes as T
                    where S.ID = T.ID));


select T.course_id
from course as T
where (select count(*) from (select distinct R.course_id
			  from section as R
			  where (T.course_id = R.course_id)
              and (R.year = 2009)) as dist) = (select count(*) from (select U.course_id
			  from section as U
			  where (T.course_id = U.course_id)
              and (U.year = 2009)) as form);
              
              
select T.course_id
from (select R.course_id
	from section as R
	where R.year = 2009) as T
group by T.course_id
having count(*) = 1;

/*
get not duplicated rows. mysql = sucks..

select T.course_id, count(*)
from (select R.course_id
from section as R
where R.year = 2009) as T
group by T.course_id
having count(*) = 1;
*/

/*
get not unique columns..

SELECT my_column, COUNT(*) as count
FROM my_table
GROUP BY my_column
HAVING COUNT(*) > 1
*/

select T.dept_name, T.avg_sal as avg_salary
from (select S.dept_name, avg(S.salary) as avg_sal
	  from instructor as S
      group by S.dept_name) as T
where T.avg_sal > 42000;




select T.dept_name, T.avg_salary
from (select S.dept_name, avg(salary) as avg_salary
	  from instructor as S
      group by S.dept_name) as T
where T.avg_salary > 42000;

select dept_name, max(budget)
from department
group by dept_name;

select dept_name, sum(salary)
from instructor
group by dept_name
having sum(salary) >= some (select avg(salary)
	   from instructor);

select dept_name,
   (select count(*) 
    from instructor 
    where department.dept_name = instructor.dept_name) as num_instructors
from department;

delete from instructor;

delete from instructor
where dept_name='Finance';

delete from instructor
where dept_name in (select dept_name
					from department
					where building = 'Watson');
                    
delete from instructor
where salary < (select * from (select avg(salary) from instructor as S) as T);

insert into course
	values('CS-437', 'Database Systems', 'Comp. Sci.', 4);
    
insert into course(course_id, title, dept_name, credits)
	values('CS-437', 'Database Systems', 'Comp. Sci.', 4);
    
insert into student
	values('3003', 'Green', 'Finance', null);

insert into student
	select ID, name, dept_name, 0
    from instructor;

update instructor
	set salary = salary * 1.03
    where salary > 100000;
    
update instructor
	set salary = salary * 1.05
    where salary <= 100000;
    
update instructor
	set salary = case
					when salary <= 100000 then salary * 1.05
                    else salary * 1.03
				 end;
                 
update student S
	set tot_cred = (select sum(credits)
					from takes natural join course
                    where S.ID = takes.ID and
						takes.grade <> 'F' and
                        takes.grade is not null);

update student S
	set tot_cred = (select (case when sum(credits) is not null then sum(credits) else 0 end)
					from takes natural join course
                    where S.ID = takes.ID and
						takes.grade <> 'F' and
                        takes.grade is not null);
                        
select * from takes natural join course order by course_id;
