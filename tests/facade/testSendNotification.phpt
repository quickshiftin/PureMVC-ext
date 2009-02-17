--TEST--
ensure Facade::sendNotification correctly creates Notification
--FILE--
<?php
class MyFacade extends Facade {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function notifyObservers($notification) {
		var_dump($notification);
	}
}
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
