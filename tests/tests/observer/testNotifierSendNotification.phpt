--TEST--
lets see if we can call the Facade::sendNotification successfully
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$n = new MyNotifier();
$n->sendNotification('blah');
$n->sendNotification('blah', 'testbody');
$n->sendNotification('blah', 'testbody', 'testtype');
?>
--EXPECT--
object(Notification)#6 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(0) ""
}
object(Notification)#7 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(8) "testbody"
}
object(Notification)#8 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(8) "testtype"
  ["body:private"]=>
  string(8) "testbody"
}
