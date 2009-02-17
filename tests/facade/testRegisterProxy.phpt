--TEST--
ensure we can store a command via Facade::registerProxy
--FILE--
<?php
class MyModel extends Model {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function registerProxy($proxy) {
		var_dump($proxy);
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
			$this->model = MyModel::getInstance();
	}
}
$f = Facade::getInstance();
$f->registerProxy(new Proxy());
--EXPECT--
