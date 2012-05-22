<?php
/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

require_once '../bootstrap.php';
require_once 'config/init.php';
require_once 'module/common/Tpl.php';
require_once 'module/common/Conn.php';


session_start();
header_html();

$is_success = true;

/* check dir permission */
$dirs = array();
$dirs[] = array('name' => '临时文件', 'path' => '/temp');
$dirs[] = array('name' => '模板缓存', 'path' => '/temp/cache');
$dirs[] = array('name' => '模板编译', 'path' => '/temp/compile');
$dirs[] = array('name' => '数据库', 'path' => '/database');
// is_writable

$dir_check_result = null;
foreach ($dirs as $dir) {
    $msg = '<span class="success">可写</span>';
    if (!is_writable(DIR_ROOT . $dir['path'])) {
        $is_success &= false;
        $msg = '<span class="error">不可写</span>';
    }
    $dir_check_result .= sprintf('<li>%s %s</li>', $msg, $dir['name']);
}

/* zoeey loaded */
$zoeey_check_result = '<span class="error">ZoeeyPHP 加载失败</span>';
if (extension_loaded('zoeey')) {
    $zoeey_check_result = '<span class="success">ZoeeyPHP 加载成功</span>';
}

/* database check */
$recorder = new ZeRecorder(Conn::getConn());
$recorder->query('DROP TABLE IF EXISTS news_e')->execute();

$recorder->query('CREATE TABLE news_e (
                          id integer PRIMARY KEY autoincrement
                        , title VARCHAR(300)
                        , category INT(11)
                        , detail VARCHAR(3000)
                        , edit_time INT(11)
                        , create_time INT(11)
                        )  ')->execute();
$time = time();
for ($i = 1; $i <= 50; $i++) {
    $recorder->query('INSERT INTO news_e values (null,:title,:category,:detail,:time,:time)')
            ->bind(array(
                'title' => 'title_' . $i
                , 'detail' => 'content_' . $i
                , 'category' => array_rand(array(1, 2, 3))
                , 'time' => $time
            ))
            ->execute();
}

$rows = $recorder->query('SELECT * FROM news_e LIMIT 0,5')->fetchAll();
$database_check_result = null;
if (count($rows) == 5) {
    $database_check_result .= '<li>* 访问成功</li>';
    foreach ($rows as $row) {
        $database_check_result .= sprintf('<li>%s</li>', $row['title']);
    }
} else {
    $is_success &= false;
    $database_check_result = '<li>* <span class="error">访问失败，请检查数据库目录权限或PDO_SQLite。</span></li>';
}
$install_msg = $is_success ? '安装成功，<a href="/index.php">返回首页。</a>' : '安装失败，请检查相关项。';
if ($is_success) {
    touch(DIR_ROOT . '/temp/lock');
}
?><!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8" />
        <title>ZoeeyPHP_Example Install</title>
        <link rel="stylesheet" href="/style/reset.css" />
        <link rel="stylesheet" href="/style/text.css" />
        <link rel="stylesheet" href="/style/960.css" />
        <link rel="stylesheet" href="/style/demo.css" />
    </head>
    <body>
        <h1>
        </h1>
        <div class="container_16">
            <h2>
                ZoeeyPHP_Example Install
            </h2>
            <div class="grid_16">
                <p><?php echo $zoeey_check_result; ?> </p>
            </div>
            <div class="grid_16">
                <p>目录权限检查</p>
            </div>
            <div class="grid_16">
                <ol>
                    <?php echo $dir_check_result; ?>
                </ol>
            </div>

            <div class="grid_16">
                <p>数据库操作检查</p>
                <ol>
                    <?php echo $database_check_result; ?>
                </ol>
            </div>

            <div class="clear"></div>

            <div class="grid_16">
                <p>
                    <?php echo $install_msg; ?>
                </p>
            </div>

        </div>
    </div>
</div>
</body>
</html>