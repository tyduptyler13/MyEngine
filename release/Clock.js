/**
 * @author alteredq / http://alteredqualia.com/
 * @author tyduptyler13
 * Modified for use in nodejs
 */

function Clock( autoStart ) {

	this.autoStart = ( autoStart !== undefined ) ? autoStart : true;

	this.startTime = 0;
	this.oldTime = 0;
	this.elapsedTime = 0;

	this.running = false;

}

Clock.prototype = {

	constructor: Clock,

	start: function () {

		this.startTime = Date.now();

		this.oldTime = this.startTime;
		this.elapsedTime = 0;
		this.running = true;

	},

	stop: function () {

		this.getElapsedTime();
		this.running = false;

	},

	getElapsedTime: function () {

		this.getDelta();
		return this.elapsedTime;

	},

	getDelta: function () {

		var diff = 0;

		if ( this.autoStart && ! this.running ) {

			this.start();

		}

		if ( this.running ) {

			var newTime = Date.now();

			diff = ( newTime - this.oldTime ) / 1000;
			this.oldTime = newTime;

			this.elapsedTime += diff;

		}

		return diff;

	}

};

module.exports = Clock;
