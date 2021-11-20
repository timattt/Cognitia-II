create table users (
	name varchar(20) primary key,
	role varchar(10),
	path varchar(300)
);

insert into users values(
	'Danila',
	'student',
	'Danila'
);

insert into users values(
	'Andrey',
	'student',
	'Andrey'
);

insert into users values(
	'Mentor',
	'mentor',
	'Mentor'  -- I have no idea what for
);

.save users.db
