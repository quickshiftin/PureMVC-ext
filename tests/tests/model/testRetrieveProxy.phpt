--TEST--
put an IProxy instance in proxyMap, and see if we can get it back out via retrieveProxy
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
		$this->proxyMap[] = new MyProxy();
	}
}
var_dump(MyModel::getInstance()->retrieveProxy('testProxy'));
?>
--EXPECT--
