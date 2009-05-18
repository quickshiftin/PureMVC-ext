<?php
class MyModel extends Model {
	private static $shouldUseMyProxy = false;

	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public static function useMyProxy() {
		self::$shouldUseMyProxy = true;
	}

	public function initializeModel() {
		if(self::$shouldUseMyProxy) {
			$p = new MyProxy();
			$this->proxyMap[$p->getProxyName()] = $p;
		}
	}

	public function hasProxy($notificationName) {
		echo $notificationName . PHP_EOL;
	}

/*
	public function retrieveProxy($proxyName) {
		echo $proxyName . PHP_EOL;
		return parent::retrieveProxy($proxyName);
	}
*/
	public function registerProxy($proxy) {
		echo get_class($proxy) . PHP_EOL;
	}

	public function removeProxy($proxyName) {
		echo $proxyName . PHP_EOL;
	}
}
