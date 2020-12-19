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

DELIMITER $$
create function ch5_p43_while()
returns INTEGER
DETERMINISTIC
begin
	 declare n INTEGER default 0;
     while n < 10 do
		set n = n + 1;
	 end while;
     return n;
end $$
DELIMITER ;

select ch5_p43_while();

DELIMITER $$
create function ch5_p43_repeat()
returns INTEGER
DETERMINISTIC
begin
	 declare n INTEGER default 10;
     repeat
		set n = n - 1;
	 until n = 0
     end repeat;
     return n;
end $$
DELIMITER ;

select ch5_p43_repeat();

DELIMITER $$
create function ch5_p43_for()
returns INTEGER
NOT DETERMINISTIC
begin
	 declare n INTEGER default 0;
     declare r INTEGER;
     declare finished INTEGER default 0;
     declare loop_cursor cursor for (select budget
		from department
        where dept_name = 'Music');
	 declare continue handler 
     FOR NOT FOUND SET finished = 1;
     
	 open loop_cursor;
     simple_loop: LOOP
		fetch loop_cursor into r;
		if finished = 1 then
			leave simple_loop;
        end if;
        set n = n - r;
	end loop simple_loop;
    close loop_cursor;
    return n;
end $$
DELIMITER ;

select ch5_p43_for();


DELIMITER $$
create procedure registerStudent(
	in s_id varchar(5),
    in s_courseid varchar(8),
    in s_secid varchar(8),
    in s_semester varchar(6),
    in s_year numeric(4,0),
    out errorMsg varchar(100),
    out result integer)
proc_label:begin
	declare currEnrol INTEGER;
    declare limits INTEGER;
    select count(*) into currEnrol
    from takes
    where course_id = s_courseid and sec_id = s_secid
    and semester = s_semester and year = s_year;
    
    select capacity into limits
		from classroom natural join section
        where course_id = s_courseid and sec_id = s_secid
        and semester = s_semester and year = s_year;
	
    if (currEnrol < limits) then
		begin
				insert into takes values
					(s_id, s_courseid, s_secid, s_semester, s_year, null);
				set result = 0;
                leave proc_label;
		end;
	end if;
    set errorMsg = 'Enrollment limit reached for course' || s_courseid || 'section' || s_secid;
    set result = -1;
end $$
DELIMITER ;

DELIMITER $$
create function ch5_p44(
	id varchar(5), 
    courseid varchar(8), 
    secid varchar(8),
    semester varchar(6),
    year numeric(4,0))
returns varchar(150)
begin
    declare errorMsg varchar(100);
    declare result integer;
	call registerStudent(id, courseid, secid, semester, year, errorMsg, result);
    return CONCAT(result, errorMsg);
end $$
DELIMITER ;

select ch5_p44('19991', 'CS-101', '1', 'Fall', '2009');


DELIMITER $$
create trigger time_slot_check after insert on section
for each row
begin
	if NEW.time_slot_id not in (select time_slot_id from time_slot) then
		begin
			rollback;
		end;
	end if;
end $$
DELIMITER ;
