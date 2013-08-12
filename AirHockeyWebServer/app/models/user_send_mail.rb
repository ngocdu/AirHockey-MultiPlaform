class UserSendMail < ActiveRecord::Base
  attr_accessible :email, :name
    VALID_EMAIL_REGEX = /\A[\w+\-.]+@[a-z\d\-.]+\.[a-z]+\z/i
    validates :email, format: {with: VALID_EMAIL_REGEX}
    validates :name, presence: true
end
