--TEST--
lets see if we can call the Facade::sendNotification successfully
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyView::makeVerbose();
$n = new MyNotifier();
$n->sendNotification('blah');
$n->sendNotification('blah', 'testbody');
$n->sendNotification('blah', 'testbody', 'testtype');
?>
--EXPECT--
Notification
Notification
Notification
