<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */


require_once 'bootstrap.php';
require_once 'config/init.php';
require_once 'module/common/Tpl.php';
/* check install */
if (!is_file(DIR_ROOT . '/temp/lock')) {
    redirect('install/');
}

session_start();
header_html();

/* @var $router Router */
$router = new ZeRouter();
$router->add('/:action');
$router->addArray('action', array('add'), '{/}');
$router->addArray('action', array('list', 'search'), '/:page')->append('{/}', '-');
$router->addArray('action', array('view', 'edit', 'del'), '/:id');
$values = $router->parse(getenv('PATH_INFO'));

/* @var $loader Loader global */
$loader = new ZeLoader();
$loader->setValues($values);

/* get user info here */

/* @var $supervisor Supervisor global */
$supervisor = new Supervisor();

require_once 'module/news/News.php';


News::editor();

$tpl = new Tpl(false);
$tpl_vars = array();

$tpls = array();
$tpls[Tpl::SN_NOTI] = 'common.noti.html';
$tpls[Tpl::SN_LIST] = 'news.list.html';
$tpls[Tpl::SN_VIEW] = 'news.view.html';
$tpls[Tpl::SN_ADD] = 'news.edit.html';
$tpls[Tpl::SN_EDIT] = 'news.edit.html';

$tpl_path = $tpls[$supervisor->tplSn];
$compile_id = dechex(crc32($tpl_path));
$status = $supervisor->status;
$info = $supervisor->info;
$target = '/news.php';
$status_name = $status->getName();
if ($status_name == 'error.nopower') {
    $target = null;
}

$tpl_vars['status'] = $status->toArray();
$tpl_vars['jumpto'] = $target;
$tpl_vars['info'] = $info;
$tpl_vars['categories'] = require_once 'module/news/news_category.php';

$tpl->assignAll($tpl_vars);

$tpl->show(Config::TPL_DIR_MANAGE, $tpl_path, null, $compile_id);
?>