--TEST--
Check for ZePageSet
--SKIPIF--
<?php
if (!extension_loaded("zoeey")) {
    print "skip";
}
?>
--FILE--
<?php
require_once 'config.php';

/**
 * between 包含左右端
 * SELECT * FROM `article` WHERE id between 0 and 3;
 * SELECT * FROM `article` WHERE id between 1 and 3;
 * +----+-------+
 * | id | title |
 * +----+-------+
 * |  1 | 111   |
 * |  2 | 222   |
 * |  3 | 333   |
 * +----+-------+
 * limit 不包含最前端
 * SELECT * FROM `article` LIMIT 1,3;
 * +----+-------+
 * | id | title |
 * +----+-------+
 * |  2 | 222   |
 * |  3 | 333   |
 * |  4 | 444   |
 * +----+-------+
 * SELECT * FROM `article` LIMIT 0,3;
 * +----+-------+
 * | id | title |
 * +----+-------+
 * |  1 | 111   |
 * |  2 | 222   |
 * |  3 | 333   |
 * +----+-------+
 */
/**
 * 记录数为0时的一些特殊情况
 */
$pageSet = new ZePageSet(3, 1);
$pageSet->setRecordCount(0);
echo '3,1,0:';
/**
 * true
 */
echo assert($pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert(!$pageSet->hasList());
/**
 * false
 */
echo ',', assert(!$pageSet->hasNext());
echo ',', assert(!$pageSet->hasPrev());
/**
 * 记录数为 0 时 当前页和页码列均只有 1 。
 * 起始列与终止列均为 0 
 */
echo ',', assert($pageSet->current() == 1);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 0);
echo ',', assert($pageSet->getStartRow() == 0);
echo ',', assert($pageSet->getEndRow() == 0);
echo ',', assert($pageSet->getRecordCount() == 0);
echo ',', assert($pageSet->siblings(0) == array(1));
echo ',', assert($pageSet->siblings(1) == array(1));
echo ',', assert($pageSet->siblings(5) == array(1));
echo ',', assert($pageSet->all() == array(1)), EOL;

/**
 * 
 */
$pageSet = new ZePageSet(3, 1);
$pageSet->setRecordCount(1);
echo '3,1,1:';
echo assert($pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert(!$pageSet->hasNext());
echo ',', assert(!$pageSet->hasPrev());
echo ',', assert($pageSet->current() == 1);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 0);
echo ',', assert($pageSet->getStartRow() == 1);
echo ',', assert($pageSet->getEndRow() == 1);
echo ',', assert($pageSet->getRecordCount() == 1);
echo ',', assert($pageSet->siblings(5) == array(1));
echo ',', assert($pageSet->all() == array(1)), EOL;
/**
 * 
 */
$pageSet = new ZePageSet(3, 1);
$pageSet->setRecordCount(3);
echo '3,1,3:';
echo assert($pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert(!$pageSet->hasNext());
echo ',', assert(!$pageSet->hasPrev());
echo ',', assert($pageSet->current() == 1);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 0);
echo ',', assert($pageSet->getStartRow() == 1);
echo ',', assert($pageSet->getEndRow() == 3);
echo ',', assert($pageSet->getRecordCount() == 3);
echo ',', assert($pageSet->siblings(1) == array(1));
echo ',', assert($pageSet->siblings(5) == array(1));
echo ',', assert($pageSet->all() == array(1)), EOL;
/**
 * 
 */
$pageSet = new ZePageSet(3, 2);
$pageSet->setRecordCount(4);
echo '3,2,4:';
echo assert(!$pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert(!$pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 2);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 3);
echo ',', assert($pageSet->getStartRow() == 4);
echo ',', assert($pageSet->getEndRow() == 4);
echo ',', assert($pageSet->getRecordCount() == 4);
//        echo ',',assert($pageSet->siblings(0) == array(2));
echo ',', assert($pageSet->siblings(1) == array(2));
echo ',', assert($pageSet->siblings(5) == array(1, 2));
echo ',', assert($pageSet->all() == array(1, 2)), EOL;
/**
 *
 */
$pageSet = new ZePageSet(3, 2);
$pageSet->setRecordCount(5);
echo '3,2,5:';
echo assert(!$pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert(!$pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 2);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 3);
echo ',', assert($pageSet->getStartRow() == 4);
echo ',', assert($pageSet->getEndRow() == 5);
echo ',', assert($pageSet->getRecordCount() == 5);
echo ',', assert($pageSet->siblings(1) == array(2));
echo ',', assert($pageSet->siblings(5) == array(1, 2));
echo ',', assert($pageSet->all() == array(1, 2)), EOL;
/**
 * 信息总量与单页信息数整除
 */
$pageSet = new ZePageSet(3, 2);
$pageSet->setRecordCount(6);
echo '3,2,6:';
echo assert(!$pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert(!$pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 2);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 3);
echo ',', assert($pageSet->getStartRow() == 4);
echo ',', assert($pageSet->getEndRow() == 6);
echo ',', assert($pageSet->getRecordCount() == 6);
echo ',', assert($pageSet->siblings(1) == array(2));
echo ',', assert($pageSet->siblings(5) == array(1, 2));
echo ',', assert($pageSet->all() == array(1, 2)), EOL;
/**
 * 页码越过最大
 */
$pageSet = new ZePageSet(3, 3);
$pageSet->setRecordCount(6);
echo '3,3,6:';
echo assert(!$pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert(!$pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 2);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getStartRow() == 4);
echo ',', assert($pageSet->getEndRow() == 6);
echo ',', assert($pageSet->getRecordCount() == 6);
echo ',', assert($pageSet->siblings(1) == array(2));
echo ',', assert($pageSet->siblings(5) == array(1, 2));
echo ',', assert($pageSet->all() == array(1, 2)), EOL;
/**
 * 当前页靠近两端
 */
$pageSet = new ZePageSet(3, 2);
$pageSet->setRecordCount(7);
echo '3,2,7:';
echo assert(!$pageSet->isFirst());
echo ',', assert(!$pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert($pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 2);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 3);
echo ',', assert($pageSet->getStartRow() == 4);
echo ',', assert($pageSet->getEndRow() == 6);
echo ',', assert($pageSet->getRecordCount() == 7);
echo ',', assert($pageSet->siblings(1) == array(2));
echo ',', assert($pageSet->siblings(5) == array(1, 2, 3));
echo ',', assert($pageSet->all() == array(1, 2, 3)), EOL;
/**
 * 当前页在中间
 */
$pageSet = new ZePageSet(3, 5);
$pageSet->setRecordCount(30);
echo '3,5,30:';
echo assert(!$pageSet->isFirst());
echo ',', assert(!$pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert($pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 5);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 12);
echo ',', assert($pageSet->getStartRow() == 13);
echo ',', assert($pageSet->getEndRow() == 15);
echo ',', assert($pageSet->getRecordCount() == 30);
echo ',', assert($pageSet->siblings(1) == array(5));
echo ',', assert($pageSet->siblings(5) == array(3, 4, 5, 6, 7));
echo ',', assert($pageSet->siblings(6) == array(3, 4, 5, 6, 7, 8));
echo ',', assert($pageSet->all() == array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)), EOL;
/**
 * 当前页靠近前端
 */
$pageSet = new ZePageSet(3, 3);
$pageSet->setRecordCount(30);
echo '3,3,30:';
echo assert(!$pageSet->isFirst());
echo ',', assert(!$pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert($pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 3);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 6);
echo ',', assert($pageSet->getStartRow() == 7);
echo ',', assert($pageSet->getEndRow() == 9);
echo ',', assert($pageSet->getRecordCount() == 30);
echo ',', assert($pageSet->siblings(1) == array(3));
echo ',', assert($pageSet->siblings(5) == array(1, 2, 3, 4, 5));
echo ',', assert($pageSet->siblings(6) == array(1, 2, 3, 4, 5, 6));
echo ',', assert($pageSet->siblings(7) == array(1, 2, 3, 4, 5, 6, 7));
echo ',', assert($pageSet->all() == array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)), EOL;
/**
 * 当前页靠近后端
 */
$pageSet = new ZePageSet(3, 8);
$pageSet->setRecordCount(30);
echo '3,8,30:';
echo assert(!$pageSet->isFirst());
echo ',', assert(!$pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert($pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 8);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 21);
echo ',', assert($pageSet->getStartRow() == 22);
echo ',', assert($pageSet->getEndRow() == 24);
echo ',', assert($pageSet->getRecordCount() == 30);
echo ',', assert($pageSet->siblings(1) == array(8));
echo ',', assert($pageSet->siblings(5) == array(6, 7, 8, 9, 10));
echo ',', assert($pageSet->siblings(6) == array(5, 6, 7, 8, 9, 10));
echo ',', assert($pageSet->siblings(7) == array(4, 5, 6, 7, 8, 9, 10));
echo ',', assert($pageSet->all() == array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)), EOL;
/**
 * 当前页靠近后端
 */
$pageSet = new ZePageSet(3, 10);
$pageSet->setRecordCount(30);
echo '3,10,30:';
echo assert(!$pageSet->isFirst());
echo ',', assert($pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert(!$pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 10);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 27);
echo ',', assert($pageSet->getStartRow() == 28);
echo ',', assert($pageSet->getEndRow() == 30);
echo ',', assert($pageSet->getRecordCount() == 30);
echo ',', assert($pageSet->siblings(1) == array(10));
echo ',', assert($pageSet->siblings(5) == array(6, 7, 8, 9, 10));
echo ',', assert($pageSet->siblings(6) == array(5, 6, 7, 8, 9, 10));
echo ',', assert($pageSet->siblings(7) == array(4, 5, 6, 7, 8, 9, 10));
echo ',', assert($pageSet->all() == array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)), EOL;
/**
 * 当前页靠近后端
 */
$pageSet = new ZePageSet(3, 9);
$pageSet->setRecordCount(30);
echo '3,9,30:';
echo assert(!$pageSet->isFirst());
echo ',', assert(!$pageSet->isLast());
echo ',', assert($pageSet->hasList());
echo ',', assert($pageSet->hasNext());
echo ',', assert($pageSet->hasPrev());
echo ',', assert($pageSet->current() == 9);
echo ',', assert($pageSet->first() == 1);
echo ',', assert($pageSet->getOffset() == 24);
echo ',', assert($pageSet->getStartRow() == 25);
echo ',', assert($pageSet->getEndRow() == 27);
echo ',', assert($pageSet->getRecordCount() == 30);
echo ',', assert($pageSet->siblings(1) == array(9));
echo ',', assert($pageSet->siblings(5) == array(6, 7, 8, 9, 10));
echo ',', assert($pageSet->siblings(6) == array(5, 6, 7, 8, 9, 10));
echo ',', assert($pageSet->siblings(7) == array(4, 5, 6, 7, 8, 9, 10));
echo ',', assert($pageSet->all() == array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
?>
--EXPECT--
3,1,0:1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,1,1:1,1,1,1,1,1,1,1,1,1,1,1,1
3,1,3:1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,2,4:1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,2,5:1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,2,6:1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,3,6:1,1,1,1,1,1,1,1,1,1,1,1,1
3,2,7:1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,5,30:1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,3,30:1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,8,30:1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,10,30:1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
3,9,30:1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1