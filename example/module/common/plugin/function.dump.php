<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

/**
 * 导出所有参数
 */
function smarty_function_dump($params, &$smarty) {
    echo '<pre>';
    foreach ($params as $param) {
        var_export($param);
    }
    echo '</pre>';
    return;
}

?>