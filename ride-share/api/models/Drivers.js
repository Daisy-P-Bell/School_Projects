/*
    Ride Share ORM
    Drivers Model
    COS 243
    Daisy Bell & Matthias Dean
*/

const { Model } = require("../db");

class Drivers extends Model {
  static get tableName() {
    return "Drivers";
  }
  static get relationMappings() {
    return {
      driver: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Driver"),
        join: {
          from: "Drivers.driver_id",
          to: "Driver.driver_id",
        },
      },
      rides: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Ride"),
        join: {
          from: "Drivers.ride_id",
          to: "Ride.ride_id",
        },
      },
    };
  }
}

module.exports = Drivers;
