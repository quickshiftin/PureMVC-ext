<?php
class MyView extends View {
	public static $isVerbose = false;
	public static $showName = false;

	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public static function makeVerbose() {
		self::$isVerbose = true;
	}

	public static function showName() {
		self::$showName = true;
	}

	public function hasMediator($notificationName) {
		echo $notificationName . PHP_EOL;
		return parent::hasMediator($notificationName);
	}

	public function registerMediator($mediator) {
		if(self::$isVerbose)
			var_dump($mediator);
		return parent::registerMediator($mediator);
	}

	public function registerObserver( $notificationName, IObserver $observer ) {
		if(self::$isVerbose)
			var_dump(func_get_args());
		if(self::$showName)
			echo "notificationName: $notificationName" . PHP_EOL;
		parent::registerObserver($notificationName, $observer);
	}

	public function retrieveMediator($notificationName) {
		echo $notificationName . PHP_EOL;
		return parent::retrieveMediator($notificationName);
	}

	public function removeMediator($notificationName) {
		echo $notificationName . PHP_EOL;
		return parent::removeMediator($notificationName);
	}

	public function notifyObservers($inotification) {
		echo get_class($inotification) . PHP_EOL;
		return parent::notifyObservers($inotification);
	}
}
