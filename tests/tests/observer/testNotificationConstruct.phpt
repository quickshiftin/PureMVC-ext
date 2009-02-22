--TEST--
let's see if the various parameters provided by the constructor work
--FILE--
<?php
$n = new Notification('blah');
var_dump($n);
$n = new Notification('blah', 'testbody');
var_dump($n);
$n = new Notification('blah', 'testbody', 'testtype');
var_dump($n);
?>
--EXPECT--
object(Notification)#1 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(0) ""
}
object(Notification)#2 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(8) "testbody"
}
object(Notification)#1 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(8) "testtype"
  ["body:private"]=>
  string(8) "testbody"
}
