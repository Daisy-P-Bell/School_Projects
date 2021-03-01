/*
    Ride Share ORM 
    Authorization Model
    COS 243
    Daisy Bell & Matthias Dean
*/

const { Model } = require("../db");

class Authorization extends Model {
  static get tableName() {
    return "Authorization";
  }
  static get relationMappings() {
    return {
      drivers: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Driver"),
        join: {
          from: "Authorization.driver_id",
          to: "Driver.driver_id",
        },
      },
      vehicles: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./Vehicle"),
        join: {
          from: "Authorization.vehicle_id",
          to: "Vehicle.vehicle_id",
        },
      },
    };
  }
}

module.exports = Authorization;
