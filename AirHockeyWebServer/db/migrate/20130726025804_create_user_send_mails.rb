class CreateUserSendMails < ActiveRecord::Migration
  def change
    create_table :user_send_mails do |t|
      t.string :name
      t.string :email

      t.timestamps
    end
  end
end
