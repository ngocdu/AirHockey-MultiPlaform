class CreateUser3s < ActiveRecord::Migration
  def change
    create_table :user3s do |t|
      t.integer :user_id
      t.integer :point
      t.integer :reward
      t.string :name
      t.string :email

      t.timestamps
    end
  end
end
