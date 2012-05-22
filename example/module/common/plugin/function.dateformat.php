<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

/**
 *
 * 日期时间格式化
 *
 * @param array $params
 * @param Tpl $tpl
 * @return string
 */
function smarty_function_dateformat($params, &$tpl) {
    $time = $tpl->getArg($params, 'time', 0);
    $format = $tpl->getArg($params, 'format', 'Y-m-d H:i:s');
    return date($format, $time);
}

?>
