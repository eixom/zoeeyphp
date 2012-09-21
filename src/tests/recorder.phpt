--TEST--
Check for ZeRecorder
--SKIPIF--
<?php
if (!extension_loaded("zoeey")) {
    print "skip";
}
if (!extension_loaded("PDO")) {
    print "skip";
}
if (!extension_loaded("pdo_sqlite")) {
    print "skip";
}
?>
--FILE--
<?php
require_once 'config.php';

$recorder = new ZeRecorder(get_conn());
/* database initialize */
$recorder->query('DROP TABLE IF EXISTS article')->execute();
$recorder->query('CREATE TABLE article (
                          id integer PRIMARY KEY autoincrement
                        , title VARCHAR(300)
                        , content VARCHAR(3000)) ')->execute();
$recorder->query('INSERT INTO  article values (null,\'title_first\',\'content_first\')')->execute();
$recorder->query('INSERT INTO  article values (null,\'title_second\',\'content_second\')')->execute();
$recorder->query('INSERT INTO  article values (null,\'title_third\',\'content_third\')')->execute();
echo 'lastId:';
$id = $recorder->lastId();
echo assert($id == 3), EOL;
echo 'fetch:';
$value = $recorder->query('SELECT title,content FROM article ')
        ->fetch();

echo assert($value['title'] == 'title_first');
echo ',', assert($value['content'] == 'content_first');

$value = $recorder->query('SELECT id , title, content, id + :test num FROM article WHERE id = :id ')
        ->bind(array(
            'id' => 2
            , 'test' => 126
        ))
        ->fetch();

echo ',', assert($value['num'] == 128);
echo ',', assert($value['title'] == 'title_second');
echo ',', assert($value['content'] == 'content_second');

$value = $recorder->query('SELECT :first `first`,:second `second` ')
        ->bind(array(
            'first' => 11
            , 'second' => 12
                )
                , array(
            'first' => PDO::PARAM_INT
            , 'second' => PDO::PARAM_STR
                )
        )
        ->fetch();

echo ',', assert($value['first'] == 11);
echo ',', assert($value['second'] == 12);


$value = $recorder->query('SELECT :first `first`,:second `second`,:second `third` ')
        ->bind(array(
            'first' => 11
            , 'second' => 12
        ))
        ->fetch();

echo ',', assert($value['first'] == 11);
echo ',', assert($value['second'] == 12);
echo ',', assert($value['third'] == 12), EOL;


echo 'getInt:';
$value = $recorder->query('SELECT 321 `key` ')
        ->getInt();

echo assert($value == 321), EOL;

echo 'fetchAll:';
$value = $recorder->query('SELECT * FROM article ')
        ->fetchAll();

echo assert(is_array($value));
echo ',', assert($value[1]['id'] == 2);
echo ',', assert($value[1]['title'] == 'title_second');
echo ',', assert($value[1]['content'] == 'content_second'), EOL;

echo 'execute:';
$value = $recorder->query('UPDATE article SET title = \'title_second_modified\' WHERE id = :id ')
        ->bind(array('id' => 2))
        ->execute();

echo assert($value), EOL;

echo 'exec:';
$value = $recorder->query('UPDATE article SET title = \'title_modified\' WHERE id > :id ')
        ->bind(array('id' => 1))
        ->exec();

echo assert($value == 2), EOL;

$recorder->query('UPDATE article SET title = \'title_modified\' WHERE id > :id  ')
        ->bind(array('id' => 0))
        ->execute();

$value = $recorder->affected();

assert($value == 3);

echo 'begin,rollback,commit:';

$recorder->begin();
$recorder->query('UPDATE article SET title = \'title_begin\'  ');
$recorder->execute();
$recorder->commit();

$recorder->begin();
$recorder->query('UPDATE article SET title = \'title_rollback\'  ');
$recorder->execute();
$recorder->rollback();
$value = $recorder->query('SELECT title,content FROM article ')
        ->fetch();
echo assert($value['title'] == 'title_begin');

/* error test */
try {
    $recorder->query('SELECT title,content FROM article ');
    $recorder->rollback();
    assert(false);
} catch (Exception $e) {
    echo ',', assert(true);
}
clean_db();
?>
--EXPECT--
lastId:1
fetch:1,1,1,1,1,1,1,1,1,1
getInt:1
fetchAll:1,1,1,1
execute:1
exec:1
begin,rollback,commit:1,1
