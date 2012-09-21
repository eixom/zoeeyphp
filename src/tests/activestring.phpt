--TEST--
Check for ZeActiveString
--SKIPIF--
<?php
if (!extension_loaded("zoeey")) {
    print "skip";
}
?>
--FILE--
<?php
require_once 'config.php';

/* putAll active */
echo 'putAll,active:';
$select = 'SELECT `id` ';
$count = 'SELECT count(`id`) ';
$where = 'WHERE 1=1 ';
$where_in = 'AND `id` IN (%s) OR `id` = %d ';
$activeString = new ZeActiveString();
$activeString->putAll(array('select' => $select
                          , 'count' => $count
                          , 'where_in' => $where_in));
$activeString->active('select');
echo assert($activeString->toString() == $select);

$activeString->active('count');
echo ',', assert($activeString->toString() == $select . $count);
echo ',', assert($activeString->toString('|') == $select . '|' . $count);

$activeString->active('where_in', '1,2,3', 5);
echo ',', assert($activeString->toString('|') == $select . '|' . $count . '|' . sprintf($where_in, '1,2,3', 5)), EOL;

/* put */
echo 'put:';
$activeString = new ZeActiveString();
$activeString->put('where', $where);

$activeString->active('where');
echo assert($activeString->toString() == $where);

/* put error */
$activeString = new ZeActiveString();
$activeString->put('where', $where);

$activeString->active('where_error');

echo ',', assert($activeString->toString() == ''), EOL;


/* cancel */
echo 'cancel:';
$activeString = new ZeActiveString();
$activeString->put('select', $select);
$activeString->put('where', $where);

$activeString->active('select');
$activeString->active('where');

echo assert($activeString->toString() == $select . $where);

$activeString->cancel('select');

echo ',', assert($activeString->toString() == $where);

$activeString->active('select');

echo ',', assert($activeString->toString() == $select . $where);

$activeString->cancel('where');

echo ',', assert($activeString->toString() == $select);


/* cancel noexist condition */
$activeString->cancel('where_test');

echo ',', assert($activeString->toString() == $select), EOL;



/* update condition */
echo 'update:';
$activeString = new ZeActiveString();
$activeString->put('select', $select);
$activeString->put('count', $count);

$activeString->active('select');

echo assert($activeString->toString() == $select);

$activeString->update('select', 'count');

echo ',', assert($activeString->toString() == $count), EOL;

/* clear */
echo 'clear:';
$activeString = new ZeActiveString();
$activeString->put('select', $select);
$activeString->put('count', $count);

$activeString->active('select');
$activeString->clear();

echo assert($activeString->toString() == '');

$activeString->active('select');
$activeString->active('count');
$activeString->clear();

echo ',', assert($activeString->toString() == '');
?>
--EXPECT--
putAll,active:1,1,1,1
put:1,1
cancel:1,1,1,1,1
update:1,1
clear:1,1
