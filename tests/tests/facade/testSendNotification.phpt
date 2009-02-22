--TEST--
ensure Facade::sendNotification correctly creates Notification
--FILE--
<?php
include(dirname(__FILE__) . '/../testlib/include.php');
$f = MyFacade::getInstance();
$f->sendNotification('blah');
$f->sendNotification('blah', 'testbody');
$f->sendNotification('blah', 'testbody', 'testtype');
?>
--EXPECT--
object(Notification)#5 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(0) ""
}
object(Notification)#6 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(8) "testbody"
}
object(Notification)#7 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(8) "testtype"
  ["body:private"]=>
  string(8) "testbody"
}
