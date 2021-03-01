/*
    Ride Share ORM
    Driver Model
    COS 243
    Daisy Bell & Matthias Dean
*/

const { Model } = require("../db");

class Driver extends Model {
  static get tableName() {
    return "Driver";
  }
  static get idColumn() {
    return "driver_id";
  }
  static get relationMappings() {
    return {
      users: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./User"),
        join: {
          from: "Driver.user_id",
          to: "User.user_id",
        },
      },
      authorizations: {
        relation: Model.HasManyRelation,
        modelClass: require("./Authorization"),
        join: {
          from: "Driver.driver_id",
          to: "Authorization.driver_id",
        },
      },
      drivers: {
        relation: Model.HasManyRelation,
        modelClass: require("./Drivers"),
        join: {
          from: "Driver.driver_id",
          to: "Drivers.driver_id",
        },
      },
      states: {
        relation: Model.BelongsToOneRelation,
        modelClass: require("./State"),
        join: {
          from: "Driver.licenceState",
          to: "State.abbreviation",
        },
      },
    };
  }
}

module.exports = Driver;
