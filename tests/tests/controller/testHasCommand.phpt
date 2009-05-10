--TEST--
ensure the Controller tells us we have a command once its registered
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$c = MyController::getInstance();
$c->registerCommand('MyNotification', 'MySimpleCommand');
$c->dumpCommandMap();
#var_dump($c->hasCommand('MyNotification'));
--EXPECT--
(bool)true
