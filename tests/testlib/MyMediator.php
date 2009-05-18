<?php
class MyMediator extends Mediator {
	public static $isVerbose = false;
	public static $hasExtraInterest = false;

	public static function makeVerbose() {
		self::$isVerbose = true;
	}

	public static function addExtraInterest() {
		self::$hasExtraInterest = true;
	}

	public function onRegister() {
		if(self::$isVerbose)
			echo __METHOD__ . PHP_EOL;
		return parent::onRegister();
	}

	public function listNotificationInterests() {
		$interests = array('MyNotification');

		if(self::$hasExtraInterest);
			$interests[] = 'MyOtherNotification';

		if(self::$isVerbose)
			echo __METHOD__ . PHP_EOL;

		return $interests;
	}

	public function registerObserver(IObserver $observer) {
		if(self::$isVerbose)
			echo __METHOD__ . PHP_EOL;
		return parent::registerObserver($observer);
	}

	public function handleNotification($notification) {
		if(self::$isVerbose)
			echo $notification->getName() . PHP_EOL;
		return parent::handleNotification($notification);
	}
}
