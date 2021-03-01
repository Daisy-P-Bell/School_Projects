/*
	Ride Share ORM
    User ORM Test 
	COS 243
	Daisy Bell & Matthias Dean
*/

const { knex, Model } = require("../db");
const User = require("../models/User");

User.query().withSchema('ride_share')
    .select('user_id')
    .withGraphFetched('drivers')
    .then(user => console.log(user))
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());