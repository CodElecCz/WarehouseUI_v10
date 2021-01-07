CREATE TABLE stock (
    id                  INTEGER PRIMARY KEY,
    supplier_id         INTEGER,
    storage_id          INTEGER,
    code                VARCHAR,
    material            VARCHAR,
    variant             VARCHAR,
    unit                VARCHAR,
    mass                INTEGER,
    count               INTEGER,
    availability        INTEGER,
    handlingInformation INTEGER
);

###

CREATE TABLE supplier (
    id       INTEGER PRIMARY KEY,
    company  VARCHAR,
    name     VARCHAR,
    division VARCHAR,
    city     VARCHAR,
    street   VARCHAR,
    zip      VARCHAR,
    ico      VARCHAR,
    dic      VARCHAR
);

###

CREATE TABLE storage (
  id INTEGER PRIMARY KEY, 
  name VARCHAR, 
  text VARCHAR
);

###

CREATE TABLE receipe (
    id          INTEGER  PRIMARY KEY AUTOINCREMENT,
    receipe_id  INTEGER,
    number      VARCHAR,
    state       VARCHAR,
    date        DATE,
    stock_id    INTEGER  REFERENCES stock (id),
    supplier_id INTEGER  REFERENCES supplier (id),
    quantity    INTEGER,
    storage_id  INT      REFERENCES storage (id),
    position_1  INTEGER,
    position_2  INTEGER,
    position_3  INTEGER,
    position_4  INTEGER,
    record      DATETIME,
    type        VARCHAR,
    count       INTEGER
);

###

CREATE TABLE position (
    storage_id INT REFERENCES storage (id),
    position_1 INT NOT NULL,
    position_2 INT NOT NULL,
    position_3 INT NOT NULL,
    position_4 INT NOT NULL,
    stock_id   INT REFERENCES stock (id),
    count      INT,
    CONSTRAINT id PRIMARY KEY (
        storage_id,
        position_1,
        position_2,
        position_3,
        position_4
    )
);

###

CREATE TRIGGER update_count_in
         AFTER INSERT
            ON receipe
      FOR EACH ROW
          WHEN NEW.type == 'in' AND 
               (NEW.state == 'ok' OR 
                NEW.state == 'pending') 
BEGIN
    UPDATE stock
       SET count = count + NEW.quantity
     WHERE id = NEW.stock_id;
    UPDATE receipe
       SET count = (
               SELECT count
                 FROM stock
                WHERE id = NEW.stock_id
           )
     WHERE id = NEW.id;
    INSERT OR IGNORE INTO position VALUES (
                                       NEW.storage_id,
                                       NEW.position_1,
                                       NEW.position_2,
                                       NEW.position_3,
                                       NEW.position_4,
                                       NEW.stock_id,
                                       0
                                   );
    UPDATE position
       SET count = count + NEW.quantity
     WHERE storage_id = NEW.storage_id AND 
           position_1 = NEW.position_1 AND 
           position_2 = NEW.position_2 AND 
           position_3 = NEW.position_3 AND 
           position_4 = NEW.position_4;
END;

###

CREATE TRIGGER update_count_out
         AFTER INSERT
            ON receipe
      FOR EACH ROW
          WHEN NEW.type == 'out' AND 
               (NEW.state == 'ok' OR 
                NEW.state == 'pending') 
BEGIN
    UPDATE stock
       SET count = count - NEW.quantity
     WHERE id = NEW.stock_id;
    UPDATE receipe
       SET count = (
               SELECT count
                 FROM stock
                WHERE id = NEW.stock_id
           )
     WHERE id = NEW.id;
    UPDATE position
       SET count = count - NEW.quantity
     WHERE storage_id = NEW.storage_id AND 
           position_1 = NEW.position_1 AND 
           position_2 = NEW.position_2 AND 
           position_3 = NEW.position_3 AND 
           position_4 = NEW.position_4;
END;

###

CREATE TRIGGER update_count_zero
         AFTER UPDATE
            ON position
      FOR EACH ROW
          WHEN NEW.count == 0
BEGIN
    DELETE FROM position
          WHERE storage_id = NEW.storage_id AND 
                position_1 = NEW.position_1 AND 
                position_2 = NEW.position_2 AND 
                position_3 = NEW.position_3 AND 
                position_4 = NEW.position_4;
END;

###

CREATE VIEW position_view (
    storage_id,
    position_1,
    position_2,
    position_3,
    position_4,
    count,
    stock_id,
    material,
    variant,
    mass,
    total,
    unit
)
AS
    SELECT position.storage_id,
           position.position_1,
           position.position_2,
           position.position_3,
           position.position_4,
           position.count,
           position.stock_id,
           stock.material,
           stock.variant,
           stock.mass,
           stock.count,
           stock.unit
      FROM position
           INNER JOIN
           stock ON position.stock_id = stock.id;

###
           
CREATE VIEW receipe_view AS
    SELECT receipe.storage_id,
           receipe.position_1,
           receipe.position_2,
           receipe.position_3,
           receipe.position_4,
           receipe.receipe_id,
           receipe.number,
           receipe.state,
           receipe.date,
           receipe.quantity,
           receipe.type,
           receipe.record,
           stock.id,
           stock.material,
           stock.variant,
           stock.mass,
           stock.unit
      FROM receipe
           INNER JOIN
           stock ON receipe.stock_id = stock.id;

           

