--TEST--
lets see if we call the execute() method and ensure it gets passed the INotification
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyController::disableEcho();
$c = MyController::getInstance();
$c->registerCommand('testNotification', 'MySimpleCommand');
$c->executeCommand(new MyNotification());
?>
--EXPECT--
testnotification
