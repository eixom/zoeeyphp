--TEST--
Check for ZeStatus
--SKIPIF--
<?php
if (!extension_loaded("zoeey")) {
    print "skip";
}
?>
--FILE--
<?php
require_once 'config.php';


define('ZE_INIT', 'init');
define('ZE_SUCCESS', 'success');
define('ZE_WARN', 'warning');
define('ZE_ERROR', 'error');

$status = new ZeStatus(ZE_INIT);
// label
echo 'label:', assert($status->getLabel() == ZE_INIT);
echo ',', assert(!$status->getName());
echo ',', assert(!$status->getBrief()), EOL;

// name
$status = new ZeStatus(ZE_SUCCESS, 'success.added');
echo 'name:', assert($status->getLabel() == ZE_SUCCESS);
echo ',', assert($status->getName() == 'success.added');
echo ',', assert(!$status->getBrief()), EOL;

// brief
$status = new ZeStatus(ZE_WARN, 'warn.title_unvalied', 'need title for article.');

echo 'brief:', assert($status->getLabel() == ZE_WARN);
echo ',', assert($status->getName() == 'warn.title_unvalied');
echo ',', assert($status->getBrief() == 'need title for article.');

// object
$obj = new stdClass();
$obj->enUS = 'need title for article.';
$obj->zhCN = '请认真填写文章标题。';
$status = new ZeStatus(ZE_ERROR, 'error.title_unvalied', $obj);
		
echo ',', assert($status->getBrief('enUS') == 'need title for article.');
echo ',', assert($status->getBrief('zhCN') == '请认真填写文章标题。');
echo ',', assert($status->getBrief('enUS') == 'need title for article.');
echo ',', assert($status->getBrief('zhCN') == '请认真填写文章标题。'), EOL;


$status = new ZeStatus(ZE_ERROR, 'error.title_unvalied', array(
              'en-US' => 'need title for article.'
            , 'zh-CN' => '请认真填写文章标题。'
        ));
echo 'multi-lang:', assert($status->getLabel() == ZE_ERROR);
echo ',', assert($status->getName() == 'error.title_unvalied');
echo ',', assert($status->getBrief('en-US') == 'need title for article.');
echo ',', assert($status->getBrief('zh-CN') == '请认真填写文章标题。'), EOL;

define('ZE_DEBUG', true);
echo 'debug:', assert($status->getBrief('zh-CN') == '[error.title_unvalied]请认真填写文章标题。'), EOL;

// toArray
$status = new ZeStatus(ZE_WARN, 'warn.title_unvalied', 'need title for article.');
echo 'toArray:', assert($status->toArray() == array('label' => ZE_WARN, 'name' => 'warn.title_unvalied', 'brief' => 'need title for article.')),EOL;



$status = new ZeStatus(ZE_INIT);
// label
echo 'label:', assert($status->getLabel() == ZE_INIT);
echo ',', assert(!$status->getName());
echo ',', assert(!$status->getBrief()), EOL;

// name
$status = new ZeStatus(ZE_SUCCESS, 'success.added');
echo 'name:', assert($status->getLabel() == ZE_SUCCESS);
echo ',', assert($status->getName() == 'success.added');
echo ',', assert(!$status->getBrief()), EOL;

// brief
$status = new ZeStatus(ZE_WARN, 'warn.title_unvalied', 'need title for article.');

echo 'brief:', assert($status->getLabel() == ZE_WARN);
echo ',', assert($status->getName() == 'warn.title_unvalied');
echo ',', assert($status->getBrief() == '[warn.title_unvalied]need title for article.'), EOL;

$status = new ZeStatus(ZE_ERROR, 'error.title_unvalied', array(
            'en-US' => 'need title for article.'
            , 'zh-CN' => '请认真填写文章标题。'
        ));
echo 'multi-lang:', assert($status->getLabel() == ZE_ERROR);
echo ',', assert($status->getName() == 'error.title_unvalied');

echo ',', assert($status->getBrief('en-US') == '[error.title_unvalied]need title for article.');
echo ',', assert($status->getBrief('zh-CN') == '[error.title_unvalied]请认真填写文章标题。');

echo ',', assert($status->getBrief('zh-CN') == '[error.title_unvalied]请认真填写文章标题。');

$obj = new stdClass();
$obj->enUS = 'need title for article.';
$obj->zhCN = '请认真填写文章标题。';
$status = new ZeStatus(ZE_ERROR, 'error.title_unvalied', $obj);
		
echo ',', assert($status->getBrief('enUS') == '[error.title_unvalied]need title for article.');
echo ',', assert($status->getBrief('zhCN') == '[error.title_unvalied]请认真填写文章标题。');
echo ',', assert($status->getBrief('enUS') == '[error.title_unvalied]need title for article.');
echo ',', assert($status->getBrief('zhCN') == '[error.title_unvalied]请认真填写文章标题。'), EOL;

// toArray
$status = new ZeStatus(ZE_WARN, 'warn.title_unvalied', 'need title for article.');
echo 'toArray:', assert($status->toArray() == array('label' => ZE_WARN, 'name' => 'warn.title_unvalied', 'brief' => 'need title for article.'));



?>
--EXPECT--
label:1,1,1
name:1,1,1
brief:1,1,1,1,1,1,1
multi-lang:1,1,1,1
debug:1
toArray:1
label:1,1,1
name:1,1,1
brief:1,1,1
multi-lang:1,1,1,1,1,1,1,1,1
toArray:1