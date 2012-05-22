--TEST--
Check for ZeRouter
--SKIPIF--
<?php
if (!extension_loaded("zoeey")) {
    print "skip";
}
?>
--FILE--
<?php
require_once 'config.php';
// add
$query = '/article/view/123321/title-of-article/';
$router = new ZeRouter();

$router->add(':module/:action/:id/:title');
$values = $router->parse($query);

echo 'normal:', assert($values ==
        array('module' => 'article'
            , 'action' => 'view'
            , 'id' => '123321'
            , 'title' => 'title-of-article'
)), EOL;
// shiftSep
$query = '/article/view/123321-title-of-article';
$router = new ZeRouter();
$router->add(':module/:action')->shiftSep('-');
$router->add(':id')->shiftSep('/');
$router->add(':title');
$values = $router->parse($query);

echo 'shiftSep:', assert($values ==
        array('module' => 'article'
            , 'action' => 'view'
            , 'id' => '123321'
            , 'title' => 'title-of-article'
)), EOL;

// addRegexp
$query = '/article/view/123321/title-of-article';
$router = new ZeRouter();
$router->add(':module/:action');
$router->addRegexp('action', '/^view$/i', '/:id/:title');
$values = $router->parse($query);
echo 'addRegexp:', assert($values ==
        array('module' => 'article'
            , 'action' => 'view'
            , 'id' => '123321'
            , 'title' => 'title-of-article'
)), EOL;

// addArray
$router = new ZeRouter();
$router->add(':module/:action');
$router->addArray('action', array('view', 'edit'), '/:id/:title');
//
$query = '/article/view/123321/title-of-article';
$values = $router->parse($query);
echo 'addArray:', assert($values ==
        array('module' => 'article'
            , 'action' => 'view'
            , 'id' => '123321'
            , 'title' => 'title-of-article'
));
//
$query = '/article/edit/123321/title-of-article';
$values = $router->parse($query);
echo ',', assert($values ==
        array('module' => 'article'
            , 'action' => 'edit'
            , 'id' => '123321'
            , 'title' => 'title-of-article'
));

$query = '/article/del/title-of-article';
$values = $router->parse($query);
echo ',', assert($values ==
        array('module' => 'article'
            , 'action' => 'del'));

$query = '/article/list/title-of-article';
$values = $router->parse($query);
echo ',', assert($values ==
        array('module' => 'article'
            , 'action' => 'list'
)), EOL;

// addParamCount
$router = new ZeRouter();
$router->addParamCount(5, '/:module/:action/:id/:title/:query');
//
$query = '/article/view/123321/title-of-article/?query';
$values = $router->parse($query);

echo 'addCount:', assert($values ==
        array('module' => 'article'
            , 'action' => 'view'
            , 'id' => '123321'
            , 'title' => 'title-of-article'
            , 'query' => '?query'
)), EOL;
// addAllRegexp
$router = new ZeRouter();
$router->addAllRegexp('/([a-z]+)-([a-z]+):([\d]+)\-(.*)/i', array('module', 'action', 'id', 'title'));

$query = '/article-view:123321-title-of-article';
$values = $router->parse($query);

echo 'addAllRegexp:', assert($values ==
        array('module' => 'article'
            , 'action' => 'view'
            , 'id' => '123321'
            , 'title' => 'title-of-article'
));

$router = new ZeRouter();
$router->addAllRegexp('/([a-z]+)-([a-z]+)#([\d]+)\-(.*)/i', array('module', 'action', 'id', 'title'));
// 正则表达式被修改 不匹配
$query = '/article-view:123321-title-of-article';
$values = $router->parse($query);
echo ',', assert($values ==
        array('module' => null
            , 'action' => null
            , 'id' => null
            , 'title' => null
));
// addAllRegexp
$router = new ZeRouter();
$router->addAllRegexp('/([a-z]+)-([a-z]+):([\d]+)\-(.*)/i', array('module', 'action', 'id', 'title'));
// 链接被修改 不匹配
$query = '/article-view-123321-title-of-article';
$values = $router->parse($query);

echo ',', assert($values ==
        array('module' => null
            , 'action' => null
            , 'id' => null
            , 'title' => null
)), EOL;
// 指定字符间隔键值对
$router = new ZeRouter();
$router->add('/:action');
$router->addArray('action', array('list'), '/:page')
        ->shiftSep(' ')
        ->add('{/}');
$values = $router->parse('/list/1/label/2/search/mytitle');
echo 'map:', assert($values ==
        array('action' => 'list'
            , 'page' => '1'
            , 'label' => '2'
            , 'search' => 'mytitle'
));

// 指定字符间隔键值对 不定参数赋给单独变量
$router = new ZeRouter();
$router->add('/:action');
$router->addArray('action', array('list'), '/:page')
        ->shiftSep(' ')
        ->add(':others{/}'); // prefix /  dismissed
$values = $router->parse('/list/1/label/2/search/mytitle');

echo ',', assert($values == //
        array('action' => 'list'
            , 'page' => '1'
            , 'others' => array('label' => '2'
                , 'search' => 'mytitle'
            )
));
// 指定字符间隔键值对，直接作为变量
$router = new ZeRouter();
$router->add('/:action');
$router->addArray('action', array('list'), '/:page')
        ->append('{/}', '-'); // prefix /  dismissed
$values = $router->parse('/list/1/label/2/search/mytitle');

echo ',', assert(count($router->getRules()) == 3);

echo ',', assert($values == //
        array('action' => 'list'
            , 'page' => '1'
            , 'label' => '2'
            , 'search' => 'mytitle'
)), EOL;

// 指定字符间隔数组
$router = new ZeRouter();
$router->add('/:action');
$router->addArray('action', array('list'), '/:page')
        ->add('/:ids[,]');
$values = $router->parse('/list/1/3,5,7,9');
echo 'array:', assert($values ==
        array('action' => 'list'
            , 'page' => '1'
            , 'ids' => array(3, 5, 7, 9)
)), EOL;


// end 测试
$router = new ZeRouter();
$router->add('/:action');
$router->addArray('action', array('view'), '/:id')->end()
        ->addRegexp('action', '/^[0-9a-z]+$/i', '/:subaction');

$values = $router->parse('/view/123/my-title');
echo 'end:', assert($values == //
        array('action' => 'view'
            , 'id' => '123'
));

$values = $router->parse('/steup/init_database');

echo ',', assert($values == //
        array('action' => 'steup'
            , 'subaction' => 'init_database'
));

// end 测试(无end情况)
$router = new ZeRouter();
$router->add('/:action');
$router->addArray('action', array('view'), '/:id')
        ->addRegexp('action', '/^[0-9a-z]+$/i', '/:subaction');

$values = $router->parse('/view/123/my-title');
echo ',', assert($values == //
        array('action' => 'view'
            , 'id' => '123'
            , 'subaction' => 'my-title'
)), EOL;
?>
--EXPECT--
normal:1
shiftSep:1
addRegexp:1
addArray:1,1,1,1
addCount:1
addAllRegexp:1,1,1
map:1,1,1,1
array:1
end:1,1,1