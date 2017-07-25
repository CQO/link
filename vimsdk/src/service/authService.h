#ifndef NODE_AUTH_SERVICE_
#define NODE_AUTH_SERVICE_

#include <memory>
#include <node.h>
#include <node_object_wrap.h>
#include "../imsdk/cmdExport.h"
#include "../utils/helper.h"
#include "../sdkChannel.h"

namespace service
{

class AuthService : public node::ObjectWrap
{
public:
	static void init(v8::Local<v8::Object> exports);
	static v8::Persistent<v8::Function> constructor;
	
	SdkChannel *channel() { return channel_; }
private:
	explicit AuthService(SdkChannel *channel);
	~AuthService();

	//����ʵ��
	static void instance(const v8::FunctionCallbackInfo<v8::Value> &args);

	/********************ע������********************/

	//ע������仯֪ͨ
	static void regNetChangedCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void netChangedHandler(void *data, int len, PersistentPtr cb);

	//ע�����ݿ��������֪ͨ
	static void regDbUpdatedCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void dbUpdatedHandler(void *data, int len, PersistentPtr cb);

	//ע�����µ�¼֪ͨ
	static void regReLoginCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void reLoginHandler(void *data, int len, PersistentPtr cb);

	//ע��汾����֪ͨ
	static void regUpgrageCb(const v8::FunctionCallbackInfo<v8::Value> &args);
	void upgrageHandler(void *data, int len, PersistentPtr cb);

	/**********************����**********************/

	void setLoginParam(imsdk::req_LoginParamBean &param, const std::string &user, const std::string &pwd, int8_t userType, const std::string &server);
	
	/**
	* \brief ����ע��
	* @param[in] server:string �����������ַ��������IP����
	* @param[in] name:string �û��ǳ�
	* @param[in] account:string ����ע���˺� ������ֻ��˻���ʽΪ��0086158********��
	* @param[in] passwd:string ����
	* @param[in] cb ������ս���ص� _1������Ϣ 
	*/
	static void quickRegister(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief ��ȡע����֤��
	* @param[in] account:string ����ע���˺� ������ֻ��˻���ʽΪ��0086158********��
	* @param[in] userType:number �˺�����
	* @param[in] server:string ������
	* @param[in] cb ������ս���ص� _1������Ϣ  _2��ʱ
	*/
	static void getRegCode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getCodeHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief ע���û�
	* @param[in] regCode ��֤��
	* @param[in] user    �����û���������ʹ����ʵ����
	* @param[in] pwd	 ����ע������
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void signup(const v8::FunctionCallbackInfo<v8::Value> &args);
	void signupHandler(void *data, int len, PersistentPtr cb);
	
	/**
	* \brief ��ȡ����������֤��
	* @param[in] server:string ������
	* @param[in] account:string ����ע���˺� ������ֻ��˻���ʽΪ��0086158********��
	* @param[in] cb ������ս���ص� _1������Ϣ  _2��ʱ
	*/
	static void getResetPwCode(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��������
	* @param[in] regCode ��֤��
	* @param[in] user    �����û�����Ϊ�մ�������
	* @param[in] pwd	 ����������
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void resetPasswd(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��ȡ���븴�Ӷ�
	* @param[in] cb ������ս���ص� _1������Ϣ
	*  _2�������:
	*	��8λ������С����
	*	��8λ���������λ��ʼ�����δ���:(1������룬0 ��ʾ��ѡ)
	*	1. �Ƿ����������
	*	2. �Ƿ������Сд��ĸ
	*	3. �Ƿ�����д�д��ĸ
	*	4. �Ƿ������Ӣ����ĸ
	*	5. �Ƿ�������ַ�(�����ַ�)
	*	6. �Ƿ�����ע��(1����0������)
	*/
	static void getPasswordRule(const v8::FunctionCallbackInfo<v8::Value> &args);
	void getPasswordRuleHandler(void *data, int len, PersistentPtr cb);

	/**
	*  \brief ��¼
	* @param[in] user:string �����û��� ������ֻ��˻���ʽΪ��0086158********��
	* @param[in] pwd:string ��������
	* @param[in] userType:number �˺�����
	* @param[in] server:string �����������ַ��������IP����
	* @param[in] cb ������ս���ص� _1������Ϣ  _2�û�Id
	* @return	int64 ���ص�ǰִ�еĲ���ID������ȡ���ô�ִ��
	*/
	static void login(const v8::FunctionCallbackInfo<v8::Value> &args);
	void loginHandler(void *data, int len, PersistentPtr cb);

	/**
	* \brief �Զ���¼�����������룩
	* @param[in] userid:number �����û�ID
	* @param[in] userType:number �˺�����
	* @param[in] server:string �����������ַ��������IP����
	* @param[in] cb ������ս���ص� _1������Ϣ  _2�û�Id
	* @return	int64 ���ص�ǰִ�еĲ���ID������ȡ���ô�ִ��
	*/
	static void autoLogin(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ���ߵ�¼
	* @param[in] userId:number �û�id���Ϊ�㣬��ȡ���һ�ε�¼�ɹ����û�id
	* @param[in] pwd:string ����
	* @param[in] cb ������ս���ص� _1������Ϣ
	*/
	static void offlineLogin(const v8::FunctionCallbackInfo<v8::Value> &args);
	
	/**
	* \brief �ǳ�
	* @param[in] cb ������ս���ص�  _1������Ϣ
	*/
	static void logout(const v8::FunctionCallbackInfo<v8::Value> &args);

	/**
	* \brief ��֤��¼��֤����߻�ȡ��һ����֤��
	* @param[[in] account:string �����¼�˺�
	* @param[[in] code:string ������֤������ codeΪ�����ȡ��һ����֤��
	* @param[[in] cb ������ս���ص� _1������Ϣ  _2��һ����֤ͼƬ
	*/
	static void verifyImgCode(const v8::FunctionCallbackInfo<v8::Value> &args);
	void verifyImgCodeHandler(void *data, int len, PersistentPtr cb);

	void commonHandler(void *data, int len, PersistentPtr cb);

	SdkChannel *channel_;
	int64_t registryId_;
};

} /*namespace service*/

#endif

