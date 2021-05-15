-- 学生成绩信息表(sc)
CREATE TABLE sc(
    sno varchar(10), --学号
    cno varchar(10), -- 课程号
    score int  -- 成绩
);

-- 课程信息表(course)
CREATE TABLE course(
    cno varchar(10), -- 课程号
    cname varchar(10), -- 课程名称
    credit int， -- 学分
    priorcourse varchar(10) -- 前置课程
);

-- 班级信息表(class)
CREATE TABLE class(
    classno varchar(10), -- 班级编号
    classname varchar(10), --  班级名称
    gno varchar(10) -- 年级
);

-- 学生基表信息表(student)
CREATE TABLE student(
    sno varchar(10), -- 学号
    sname varchar(10), -- 学生姓名
    gender varchar(2), -- 性别
    age int, -- 年龄
    nation varchar(10), -- 国籍
    classno varchar(10) -- 班级编号
);

-- test query sql
SELECT classno, classname, avg(score) as avg_score
FROM sc, (SELECT * FROM class WHERE class.gno = 'grade one') as sub
WHERE 
sc.sno in (SELECT sno FROM student WHERE studnet.classno = sub.classno)
and 
sc.cno in (SELECT course.cno FROM course WHERE course.cname = 'computer')
GROUP BY classno, classname
HAVING avg(score) > 60
ORDER BY avg_score;
