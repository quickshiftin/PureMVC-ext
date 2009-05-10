<?php
class MyView extends View {
	public static $isVerbose = false;

	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public static function makeVerbose() {
		self::$isVerbose = true;
	}

	public function hasMediator($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function registerMediator($mediator) {
		var_dump($mediator);
	}

	public function registerObserver( $notificationName, IObserver $observer ) {
		if(self::$isVerbose)
			var_dump(func_get_args());
		parent::registerObserver($notificationName, $observer);
	}

	public function retrieveMediator($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function removeMediator($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function notifyObservers($inotification) {
echo 'CARE' . PHP_EOL;
		echo get_class($inotification) . PHP_EOL;
	}
}
