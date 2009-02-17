--TEST--
ensure Facade::removeProxy passes the $proxyName to the controller
--FILE--
<?php
class MyModel extends Model {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function removeProxy($notificationName) {
		echo $notificationName . PHP_EOL;
	}
}
class MyFacade extends Facade {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	protected function initializeModel() {
		if(is_null($this->model))
			$this->model= MyModel::getInstance();
	}
}
$f = MyFacade::getInstance();
$f->removeProxy('MyNotification');
?>
--EXPECT--
MyNotification
