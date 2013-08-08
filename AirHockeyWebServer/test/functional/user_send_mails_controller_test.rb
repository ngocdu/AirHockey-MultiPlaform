require 'test_helper'

class UserSendMailsControllerTest < ActionController::TestCase
  setup do
    @user_send_mail = user_send_mails(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:user_send_mails)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create user_send_mail" do
    assert_difference('UserSendMail.count') do
      post :create, user_send_mail: { email: @user_send_mail.email, name: @user_send_mail.name }
    end

    assert_redirected_to user_send_mail_path(assigns(:user_send_mail))
  end

  test "should show user_send_mail" do
    get :show, id: @user_send_mail
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @user_send_mail
    assert_response :success
  end

  test "should update user_send_mail" do
    put :update, id: @user_send_mail, user_send_mail: { email: @user_send_mail.email, name: @user_send_mail.name }
    assert_redirected_to user_send_mail_path(assigns(:user_send_mail))
  end

  test "should destroy user_send_mail" do
    assert_difference('UserSendMail.count', -1) do
      delete :destroy, id: @user_send_mail
    end

    assert_redirected_to user_send_mails_path
  end
end
