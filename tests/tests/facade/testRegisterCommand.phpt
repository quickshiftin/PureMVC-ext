--TEST--
ensure we can store a command via Facade::registerCommand
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$f = MyFacade::getInstance();
$f->registerCommand('SubCommand', 'MyNotification');
?>
--EXPECT--
SubCommand MyNotification
