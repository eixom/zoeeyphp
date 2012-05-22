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
 * 链接构造函数
 *
 * @param array $params
 * @param Tpl $tpl
 */
function smarty_function_buildurl($params, &$tpl) {
    $urls = array();
    $as = $tpl->getArg($params, 'as', 'query');
    unset($params['as']);

    $prefix = $tpl->getArg($params, 'prefix', '/');
    unset($params['prefix']);

    $hides = $tpl->getArg($params, 'hide', null);
    unset($params['hide']);
    if (!empty($hides)) {
        $hides = explode(',', $hides);
        $hides = array_map('trim', $hides);
    }

    if (!empty($params)) {
        foreach ($params as $key => $val) {
            if (strlen($val) == 0) {
                continue;
            }
            if (!$hides || !in_array($key, $hides)) {
                $urls[] = urlencode($key);
            }
            $urls[] = urlencode($val);
        }
    }
    if (!empty($urls)) {
        $tpl->assign($as, $prefix . join('/', $urls));
    } else {
        $tpl->assign($as, null);
    }
}

?>
