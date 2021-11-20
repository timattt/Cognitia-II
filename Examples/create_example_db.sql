-- USERS
create table users (
	name varchar(20) primary key,
	role varchar(20)
);

insert into users values (
	"Danila",
	"student"
);

insert into users values (
	"Andrey",
	"student"
);

insert into users values (
	"Timur",
	"student"
);

insert into users values (
	"Ded",
	"mentor"
);

insert into users values (
	"Yakovlev",
	"mentor"
);

-- USERS PROGRESS
create table student_x_progress_file (
	student varchar(20) primary key,
	path varchar(500),
	foreign key (student) references users(name)
);

insert into student_x_progress_file values (
	"Danila",
	"Danila_progress"
);

insert into student_x_progress_file values (
	"Andrey",
	"Andrey_progress"
);

insert into student_x_progress_file values (
	"Timur",
	"Timur_progress"
);

-- MENTOR'S STUDENTS
create table mentor_x_student (
	mentor varchar(20),
	student varchar(20),
	primary key (mentor, student),
	foreign key (mentor) references users(name),
	foreign key (student) references users(name)
);

insert into mentor_x_student values (
	"Ded",
	"Andrey"
);

insert into mentor_x_student values (
	"Ded",
	"Timur"
);

insert into mentor_x_student values (
	"Yakovlev",
	"Danila"
);

.save example.db
