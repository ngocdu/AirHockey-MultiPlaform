class AddEmailToUser3s < ActiveRecord::Migration
  def change
    add_column :user3s, :email, :string
    add_column :user3s, :name, :string
    add_column :user3s, :reward, :integer
  end
end
