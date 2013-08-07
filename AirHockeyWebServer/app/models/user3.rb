class User3 < ActiveRecord::Base
  attr_accessible :point, :user_id, :name, :email, :reward
  
  has_many :user, dependent: :destroy
  VALID_EMAIL_REGEX = /\A[\w+\-.]+@[a-z\d\-.]+\.[a-z]+\z/i
  validates :email, format: {with: VALID_EMAIL_REGEX}
  validates :name, presence: true, uniqueness: {case_sensitive: false} 
  default_scope order: 'user3s.point DESC'
end
