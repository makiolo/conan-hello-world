#include <iostream>
#include <memory>
#include <entt/entt.hpp>
#include <boost/signals2.hpp>
#include <boost/stacktrace.hpp>
#include <signal.h>
#include <boost/poly_collection/base_collection.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

template <typename T>
class IState
{
	T get_state() const;
	T get_previous() const;
	void reset();
};

struct SwitchModel : public IState<bool>
{
	explicit SwitchModel(bool active, bool previous = false)
		: _active(active)
		, _previous(previous)
	{
		;
	}

	virtual ~SwitchModel() = default;

	bool get_state() const
	{
		return _active;
	}

	bool get_previous() const
	{
		return _previous;
	}

	void reset()
	{
		_previous = _active;
	}

	void enable()
	{
		_previous = _active;
		_active = true;
	}

	void disabled()
	{
		_previous = _active;
		_active = false;
	}

	bool _active;
	bool _previous;
};

struct AmbientModel
{
	float temperature;
};

class Logic
{
public:
	virtual void update(entt::DefaultRegistry& registry) = 0;
	virtual ~Logic() = default;
};

class AmbientLogic : public Logic
{
public:
	explicit AmbientLogic()
		: _dist(0, 2)
	{
		;
	}

	virtual void update(entt::DefaultRegistry& registry) override
	{
		registry.view<AmbientModel>().each([&](auto entity, auto &ambient) {
			ambient.temperature += _dist(_gen);
		});
	}

	boost::mt19937 _gen;
	boost::random::normal_distribution<float> _dist;
};

class FanLogic : public Logic
{
public:
	virtual void update(entt::DefaultRegistry& registry) override
	{
		registry.view<AmbientModel>().each([&](auto e1, auto &ambient) {
			registry.view<SwitchModel>().each([&](auto e2, auto &fan) {

				// causa: ambiente
				if(ambient.temperature > 26.0f)
				{
					fan.enable();
				}
				else
				{
					fan.disabled();
				}

				// causa: estado del ventilador
				if(fan.get_state())
				{
					ambient.temperature -= 0.1;
				}
				else
				{
					ambient.temperature += 0.1;
				}

			});
		});
	}
};

template <typename T>
class MachineState : public Logic
{
public:
	virtual void update(entt::DefaultRegistry& registry) override
	{
		registry.view<T>().each([&](auto entity, auto& state) {
			if(state.get_previous() != state.get_state())
			{
				change_state();
				state.reset();

				if(state.get_state())
				{
					std::cout << "ON" << std::endl;
				}
				else
				{
					std::cout << "OFF" << std::endl;
				}
			}
			else
			{
				std::cout << "=" << std::endl;
			}
		});
	}
public:
	boost::signals2::signal<void()> change_state;
};

namespace msm = boost::msm;
namespace msmf = boost::msm::front;
namespace mpl = boost::mpl;

struct NextStep {};

// ----- State machine
struct Sm1_:msmf::state_machine_def<Sm1_>
{
	struct Init:msmf::state<> {};

	struct Cooldown:msmf::state<> {
		// Entry action
		template <class Event, class Fsm>
		void on_entry(Event const&, Fsm&) const
		{
			// std::cout << "Cooldown::on_entry()" << std::endl;
		}

		// Exit action
		template <class Event, class Fsm>
		void on_exit(Event const&, Fsm&) const
		{
			// std::cout << "Cooldown::on_exit()" << std::endl;
		}
	};

	struct Execution:msmf::state<> {
		// Entry action
		template <class Event, class Fsm>
		void on_entry(Event const&, Fsm&) const
		{
			// std::cout << "Execution::on_entry()" << std::endl;
		}

		// Exit action
		template <class Event, class Fsm>
		void on_exit(Event const&, Fsm&) const
		{
			// std::cout << "Execution::on_exit()" << std::endl;
		}
	};

	struct Finishing:msmf::state<> {
		// Entry action
		template <class Event, class Fsm>
		void on_entry(Event const&, Fsm&) const
		{
			// std::cout << "Finishing::on_entry()" << std::endl;
		}

		// Exit action
		template <class Event, class Fsm>
		void on_exit(Event const&, Fsm&) const
		{
			// std::cout << "Finishing::on_exit()" << std::endl;
		}
	};
	struct End:msmf::terminate_state<> {};

	// Actions
	struct InitAction
	{
		template <class Event, class Fsm, class SourceState, class TargetState>
		void operator()(Event const&, Fsm&, SourceState&, TargetState&) const
		{
			std::cout << "InitAction()" << std::endl;
		}
	};

	struct CooldownAction
	{
		template <class Fsm, class SourceState, class TargetState>
		void operator()(NextStep const&, Fsm&, SourceState&, TargetState&) const
		{
			std::cout << "CooldownAction()" << std::endl;
		}
	};

	struct ExecutionAction
	{
		template <class Fsm, class SourceState, class TargetState>
		void operator()(NextStep const&, Fsm&, SourceState&, TargetState&) const
		{
			std::cout << "ExecutionAction()" << std::endl;
		}
	};

	struct FinishingAction
	{
		template <class Fsm, class SourceState, class TargetState>
		void operator()(NextStep const&, Fsm&, SourceState&, TargetState&) const
		{
			std::cout << "FinishingAction()" << std::endl;
		}
	};

    // Transition table
	struct transition_table:mpl::vector<
		//				Start			Event				Target			Action					Guard
		msmf::Row <		Init,			msmf::none,			Cooldown,		InitAction,				msmf::none >,
		msmf::Row <		Cooldown,		NextStep,			Execution,		CooldownAction,			msmf::none >,
		msmf::Row <		Execution,		NextStep,			Finishing,		ExecutionAction,		msmf::none >,
		msmf::Row <		Finishing,		NextStep,			End,			FinishingAction,		msmf::none >
	> {};

	// Set initial state
	using initial_state = Init;
};

using StateMachine = msm::back::state_machine<Sm1_>;

int main()
{
	try
	{
		entt::DefaultRegistry registry;

		auto ambient = registry.create();
		registry.assign<AmbientModel>(ambient, 22.0f);

		auto fan = registry.create();
		registry.assign<SwitchModel>(fan, false);

		boost::base_collection<Logic> polly;
		polly.register_types<AmbientLogic, FanLogic, MachineState<SwitchModel> >();
		polly.insert( AmbientLogic() );
		polly.insert( FanLogic() );
		polly.insert( MachineState<SwitchModel>() );
		for(int i=0; i<100;++i)
		{
			for(auto& elem : polly)
			{
				elem.update(registry);
			}
		}

		registry.destroy(fan);
	}
	catch(...)
	{
		std::cout << boost::stacktrace::stacktrace();
	}

	// estudiar HFSM: http://redboltz.wikidot.com/sub-machine-state
	StateMachine fsm;
	std::cout << "111111" << std::endl;
	fsm.start(); 
	std::cout << "222222" << std::endl;
	fsm.process_event( NextStep() );
	std::cout << "33333" << std::endl;
	fsm.process_event( NextStep() );
	std::cout << "44444" << std::endl;
	fsm.process_event( NextStep() );
	std::cout << "55555" << std::endl;
	fsm.process_event( NextStep() );
	std::cout << "66666" << std::endl;
}

