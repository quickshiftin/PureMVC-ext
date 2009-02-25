--TEST--
test the constructor to see if it initializes proxyMap, and calls the hook method
--FILE--
<?php
class MyModel extends Model {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function initializeModel() {
		var_dump($this->proxyMap);
	}
}
MyModel::getInstance();
?>
--EXPECT--
array(0) {
}
