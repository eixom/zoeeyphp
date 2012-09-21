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

$loader = ze_loader();
echo 'ze_loader:';
echo assert($loader instanceof ZeLoader);

$loader = ze_loader();
echo ',', assert($loader instanceof ZeLoader);

$loader = new ZeLoader();
echo ',', assert(ze_loader($loader) instanceof ZeLoader), EOL;

$router = ze_router();
echo 'ze_router:';
echo assert($router instanceof ZeRouter);

$router = ze_router();
echo ',', assert($router instanceof ZeRouter);

$router = new ZeRouter();
echo ',', assert(ze_router($router) instanceof ZeRouter);


?>
--EXPECT--
ze_loader:1,1,1
ze_router:1,1,1
