#ifndef ATHENA_PAXOS_CLIENT_HXX
# define ATHENA_PAXOS_CLIENT_HXX

# include <elle/With.hh>

# include <reactor/Scope.hh>

namespace athena
{
  namespace paxos
  {
    /*-------------.
    | Construction |
    `-------------*/

    template <typename T, typename ClientId>
    Client<T, ClientId>::Client(ClientId id, Peers peers)
      : _id(id)
      , _peers(std::move(peers))
      , _round(0)
    {}

    template <typename T, typename ClientId>
    T
    Client<T, ClientId>::choose(T const& value_)
    {
      T value = value_; // FIXME: no need to copy if not replaced
      ELLE_LOG_COMPONENT("athena.paxos.Client");
      ELLE_TRACE_SCOPE("%s: choose %s", *this, value);
      while (true)
      {
        ++this->_round;
        Proposal proposal{this->_round, this->_id};
        {
          boost::optional<Accepted> previous;
          int reached = 0;
          elle::With<reactor::Scope>() << [&] (reactor::Scope& scope)
          {
            for (auto& peer: this->_peers)
            {
              scope.run_background(
                elle::sprintf("paxos proposition"),
                [&]
                {
                  try
                  {
                    if (auto p = peer->propose(proposal))
                      if (!previous || previous->proposal < p->proposal)
                      {
                        ELLE_DEBUG_SCOPE("%s: value already accepted: %s",
                                         *this, p.get());
                        previous = p;
                      }
                    ++reached;
                  }
                  catch (typename Peer::Unavailable const&)
                  {}
                });
            }
            reactor::wait(scope);
          };
          if (reached <= this->_peers.size() / 2)
            throw elle::Error("too few peers are available to reach consensus");
          if (previous)
          {
            ELLE_DEBUG("%s: replace value with %s", *this, previous->value);
            value = std::move(previous->value);
          }
        }
        {
          int reached = 0;
          bool conflicted = false;
          elle::With<reactor::Scope>() << [&] (reactor::Scope& scope)
          {
            for (auto& peer: this->_peers)
            {
              scope.run_background(
                elle::sprintf("paxos proposition"),
                [&]
                {
                  try
                  {
                    auto minimum = peer->accept(proposal, value);
                    // FIXME: If the majority doesn't conflict, the value was
                    // still chosen - right ? Take that in account.
                    if (proposal < minimum)
                    {
                      conflicted = true;
                      scope.terminate_now();
                    }
                    ++reached;
                  }
                  catch (typename Peer::Unavailable const&)
                  {}
                });
            }
            reactor::wait(scope);
          };
          if (reached <= this->_peers.size() / 2)
            throw elle::Error(
              "to few peers are available to reach consensus");
          if (conflicted)
            // FIXME: we could (should) potentially skip rounds to catch up to
            // the latest - right ?
            continue;
          else
            break;
        }
      }
      ELLE_TRACE("%s: chose %s", *this, value);
      return value;
    }

    /*----------.
    | Printable |
    `----------*/

    template <typename T, typename ClientId>
    void
    Client<T, ClientId>::print(std::ostream& output) const
    {
      elle::fprintf(output, "Client(%s)", this->_id);
    }

    /*------------.
    | Unavailable |
    `------------*/

    template <typename T, typename ClientId>
    Client<T, ClientId>::Peer::Unavailable::Unavailable()
      : elle::Error("paxos peer unavailable")
    {}
  }
}

#endif
