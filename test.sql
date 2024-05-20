CREATE DATABASE db1;
CREATE SCHEMA sch1 IN db1;
USING db1.sch1;
CREATE TABLE my_table (INT id, VARCHAR(20) name);
INSERT (id, name) INTO my_table VALUES (1, 'TEST'), (2, 'ANOTHER'), (3, 'LAST');
SELECT * FROM my_table t;
SELECT * FROM my_table t WHERE t.id > 1;
-- Comment