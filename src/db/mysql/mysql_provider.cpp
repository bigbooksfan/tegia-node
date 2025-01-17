#include "mysql_provider.h"


namespace tegia { 
namespace mysql {


provider* provider::_self = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////

tegia::mysql::pool * provider::get(const std::string &name)
{
	if(_self == nullptr)
	{
		_self = new provider();
	} 

	return _self->get_pool(name);
};

/////////////////////////////////////////////////////////////////////////////////////////////

tegia::mysql::pool * provider::add(int threads_count, nlohmann::json config)
{
	if(_self == nullptr)
	{
		_self = new provider();
	}
	return _self->add_pool(threads_count, config); 
};


/////////////////////////////////////////////////////////////////////////////////////////////
provider::provider()
{

};
	
/////////////////////////////////////////////////////////////////////////////////////////////
tegia::mysql::pool * provider::get_pool(const std::string &name)
{
	auto search = this->_pools.find(name);
	if( search  != this->_pools.end() )
	{
		return search->second;
	}

	std::cout << _ERR_TEXT_ << "MYSQL POOL [" << name << "] NOT FOUND" << std::endl;
	LERROR("MYSQL POOL [" << name << "] NOT FOUND");

	return nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////
tegia::mysql::pool * provider::add_pool(int threads_count, nlohmann::json config)
{  
	std::string name = config["name"].get<std::string>();
	auto search = this->_pools.find(name);
	if( search  != this->_pools.end() )
	{
		// Пул с таким именем уже существует!!!
		return search->second;
	}

	std::string type = "mysql";
	if(config["type"].is_null() == false)
	{
		type = config["type"].get<std::string>();
	}

	this->_pools.emplace( 
		std::make_pair < std::string, tegia::mysql::pool * > ( 
			config["name"].get<std::string>(),   
			new tegia::mysql::pool( 
				type, name,
				threads_count, 
				config["database"]["host"].get<std::string>(), 
				config["database"]["port"].get<int>(),
				config["database"]["user"].get<std::string>(),
				config["database"]["password"].get<std::string>(),
				config["database"]["dbname"].get<std::string>() 
			)
		)
	);
	
	return this->_pools[name]; 
};




}   // end namespace mysql
}   // end namespace tegia


