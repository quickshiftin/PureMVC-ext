--TEST--
see if hasProxy() returns true when the IProxy instance is there
--FILE--
<?php
class MyProxy implements IProxy {
	public function getProxyName() {
		return "testProxy";
	}
	public function getData() {}
	public function setData( $data ) {}
	public function onRemove() {} 
	public function onRegister() {}
}
class MyModel extends Model {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function initializeModel() {
		$p = new MyProxy();
		$this->proxyMap[$p->getName()] = $p;
	}
}
var_dump(MyModel::getInstance()->hasProxy('testProxy'));
--EXPECT--
true
