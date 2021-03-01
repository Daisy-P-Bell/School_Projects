/*
	Ride Share ORM
	General ORM Test (non-specific)
	COS 243
	Daisy Bell & Matthias Dean
*/

const Driver = require("../models/Driver.js");

Driver.query()
	.then((drivers) => console.log(drivers));