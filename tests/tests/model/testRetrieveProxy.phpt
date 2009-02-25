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
		$p = new MyProxy();
		$this->proxyMap[$p->getProxyName()] = $p;
		var_dump($this);
	}
}
$m = MyModel::getInstance();
var_dump($m);
var_dump($m->retrieveProxy('testProxy'));
?>
--EXPECT--
object(MyModel)#1 (1) {
  ["proxyMap:protected"]=>
  array(1) {
    ["testProxy"]=>
    object(MyProxy)#2 (0) {
    }
  }
}
object(MyModel)#1 (1) {
  ["proxyMap:protected"]=>
  array(1) {
    ["testProxy"]=>
    object(MyProxy)#2 (0) {
    }
  }
}
object(MyProxy)#2 (0) {
}
