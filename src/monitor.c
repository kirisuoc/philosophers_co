/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:47:37 by ecousill          #+#    #+#             */
/*   Updated: 2025/05/26 20:10:23 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Checks if the philosopher is dead
int	philosopher_dead(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(philo->meal_lock);
	if (get_current_time() - philo->last_meal >= time_to_die
		&& philo->eating == 0)
		return (pthread_mutex_unlock(philo->meal_lock), 1);
	pthread_mutex_unlock(philo->meal_lock);
	return (0);
}

// Check if any philo died

int	check_if_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].num_of_philos)
	{
		if (philosopher_dead(&philos[i], philos[i].time_to_die))
		{
			print_message("died", &philos[i], philos[i].id);
			pthread_mutex_lock(philos[0].dead_lock);
			*philos->dead = 1;
			pthread_mutex_unlock(philos[0].dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

// Checks if all the philos ate the num_of_meals

int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finishing_eating;

	i = 0;
	finishing_eating = 0;
	if (philos[0].num_times_to_eat == -1)
		return (0);
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[i].meal_lock);
		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
			finishing_eating++;
		pthread_mutex_unlock(philos[i].meal_lock);
		i++;
	}
	if (finishing_eating == philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos->dead = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}

// Monitor thread routine

void	*monitor(void *pointer)
{
	t_philo	*philos;

	philos = (t_philo *)pointer;
	while (1)
		if (check_if_dead(philos) == 1 || check_if_all_ate(philos) == 1)
			break ;
	return (pointer);
}
