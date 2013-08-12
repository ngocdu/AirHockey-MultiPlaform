require 'test_helper'

class User3sControllerTest < ActionController::TestCase
  setup do
    @user3 = user3s(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:user3s)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create user3" do
    assert_difference('User3.count') do
      post :create, user3: { integer: @user3.integer, integer: @user3.integer, point: @user3.point }
    end

    assert_redirected_to user3_path(assigns(:user3))
  end

  test "should show user3" do
    get :show, id: @user3
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @user3
    assert_response :success
  end

  test "should update user3" do
    put :update, id: @user3, user3: { integer: @user3.integer, integer: @user3.integer, point: @user3.point }
    assert_redirected_to user3_path(assigns(:user3))
  end

  test "should destroy user3" do
    assert_difference('User3.count', -1) do
      delete :destroy, id: @user3
    end

    assert_redirected_to user3s_path
  end
end
