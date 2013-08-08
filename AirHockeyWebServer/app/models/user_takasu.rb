class UserTakasu < ActiveRecord::Base
    attr_accessible :name, :email
    VALID_EMAIL_REGEX = /\A[\w+\-.]+@[a-z\d\-.]+\.[a-z]+\z/i
    validates :email, format: {with: VALID_EMAIL_REGEX}
    validates :name, presence: true
    default_scope order: 'users.point DESC'
end
