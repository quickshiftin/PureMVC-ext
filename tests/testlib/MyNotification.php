<?php
class MyNotification implements INotification {
	static private $shouldOverrideName = true;

	public static function dissableNameOverride() {
		self::$shouldOverrideName = false;
	}

	public static function enableNameOverride() {
		self::$shouldOverrideName = true;
	}

	public function getName() {
		if(self::$shouldOverrideName)
			return 'testNotification';
		return __CLASS__;
	}

	public function getType() {}
	public function getBody() {}
	public function setName($name) {}
	public function setBody($body) {}
	public function setType($type) {}
	public function toString() {}
}
