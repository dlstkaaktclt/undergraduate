select * from student natural join takes;

select * from student s, takes t where s.ID = t.ID;

select * from student inner join takes on student.ID = takes.ID;

select * from course natural left outer join prereq;

select * from student natural left outer join takes;

select * from course natural right outer join prereq;

select * from takes natural right outer join student;

# select * from course natural full outer join prereq; 
# not working! mysql doesn't have full outer join!

select * from course natural left outer join prereq
union
select * from course natural right outer join prereq; 

# (select * from) added.
select * from course inner join prereq on course.course_id = prereq.course_id;

# (select * from) added.
select * from course left outer join prereq on course.course_id = prereq.course_id;

# (select * from) added.
select * from course natural right outer join prereq;


# select * from course full outer join prereq using (course_id);
# not working! mysql doesn't have full outer join!

select * from course left outer join prereq using(course_id)
union
select * from course right outer join prereq using(course_id);

DELIMITER $$
create function dept_count(dept_name varchar(20))
returns INTEGER
	NOT DETERMINISTIC
begin
	declare d_count INTEGER default 0;
	select count(*) into d_count
	from instructor
    where instructor.dept_name = dept_name;
    return d_count;
end $$
DELIMITER ;

select dept_name, budget
from department
where dept_count(dept_name) > 1;


DELIMITER $$
create procedure dept_count_proc(in dept_name varchar(20),
								 out d_count integer)
begin
	select count(*) into d_count
	from instructor
    where instructor.dept_name = dept_name;
end $$
DELIMITER ;

DELIMITER $$
create function using_dept_count_proc()
returns INTEGER
begin
	declare result INTEGER;
	call dept_count_proc('Biology', result);
    return result;
end $$
DELIMITER ;

select using_dept_count_proc();