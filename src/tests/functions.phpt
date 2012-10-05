--TEST--
Check for Functions
--SKIPIF--
<?php
if (!extension_loaded("zoeey")) {
    print "skip zoeey unload.";
}
?>
--FILE--
<?php
require_once 'config.php';

$request = ze_request();
echo 'ze_request:';
echo assert($request instanceof ZeRequest);

$request = ze_request();
echo ',', assert($request instanceof ZeRequest);

$request = new ZeRequest();
echo ',', assert(ze_request($request) instanceof ZeRequest), EOL;

$router = ze_router();
echo 'ze_router:';
echo assert($router instanceof ZeRouter);

$router = ze_router();
echo ',', assert($router instanceof ZeRouter);

$router = new ZeRouter();
echo ',', assert(ze_router($router) instanceof ZeRouter);


?>
--EXPECT--
ze_request:1,1,1
ze_router:1,1,1
