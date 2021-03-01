/*
	Ride Share ORM
	Location ORM Test
	COS 243
	Daisy Bell & Matthias Dean
*/

const Location = require("../models/Location.js");
const { knex, Model } = require("../db");


Location.query().withSchema('ride_share')
	.select('location_id')
	.withGraphFetched('states')
	.then(location => console.log(location))
	.catch(error => console.log(error.message))
	.then(() => knex.destroy());